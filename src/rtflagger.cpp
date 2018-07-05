#include "rtflagger.h"

#include <aoflagger.h>

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <map>
#include <cmath>
#include <complex>

#include <xmmintrin.h>

using namespace aoflagger;

Rtflagger::Rtflagger():
_flagger(0),
_strategy(0) 
{
}

Rtflagger::~Rtflagger()
{
        delete _flagger;
	delete _strategy;
}

void Rtflagger::Run(SpeadRecData SRecData)
{
        //Create the AOFlagger instance
        _flagger = new AOFlagger();
        //Specify a strategy with MakeStrategy() or LoadStrategy()
        /*AOFlagger has not the SKA_TELESCOPE Strategy, here use the MWA_TELESCOPE instead*/
        _strategy = new Strategy(_flagger->MakeStrategy(MWA_TELESCOPE));

        antennaCount = SRecData.num_stations;
        img_width = SRecData.num_baselines;
        img_height = SRecData.num_channels;
        // Initialize buffers
        for(size_t antenna1=0;antenna1!=antennaCount;++antenna1)
        {
           for(size_t antenna2=antenna1; antenna2!=antennaCount; ++antenna2)
           {              ImageSet *imageSet = new ImageSet(_flagger->MakeImageSet(img_width, img_height, 8, 0.0f));
                                 _imageSetBuffers.insert(std::pair<std::pair<size_t,size_t>, ImageSet*>(
                                 std::pair<size_t,size_t>(antenna1, antenna2), imageSet
                                ));
           }
        }

        _fullysetMask = new FlagMask(_flagger->MakeFlagMask(img_width, img_height, true));
        _correlatorMask = new FlagMask(_flagger->MakeFlagMask(img_width, img_height, false));
        flagBadCorrelatorSamples(*_correlatorMask);

        for(size_t antenna1=0;antenna1!=antennaCount;++antenna1)
        {
           for(size_t antenna2=antenna1; antenna2!=antennaCount; ++antenna2)
           {
                         _baselinesToProcess.push(std::pair<size_t,size_t>(antenna1, antenna2));
                         // We will put a place holder in the flagbuffer map, so we don't have to write (and lock)
                         //during multi threaded processing.
                         _flagBuffers.insert(std::pair<std::pair<size_t, size_t>, FlagMask*>(
                         std::pair<size_t,size_t>(antenna1, antenna2), 0));
           }
        }

        _baselinesToProcessCount = _baselinesToProcess.size();

        _processWatch.Start();

        if(!_flagFileTemplate.empty())
        {
                        _progressBar.reset(new ProgressBar("Reading flags"));
                        if(_flagReader.get() == 0)
                                _flagReader.reset(new FlagReader()); // Need to create
                        // Create the flag masks
                        for(size_t antenna1=0;antenna1!=antennaCount;++antenna1)
                        {
                                for(size_t antenna2=antenna1; antenna2!=antennaCount; ++antenna2)
                                {
                                        FlagMask* mask = new FlagMask(_flagger->MakeFlagMask(img_width, img_height));
                                        _flagBuffers.find(std::make_pair(antenna1, antenna2))->second = mask;
                                }
                        }
                        // Fill the flag masks by reading the files
                        size_t baselineIndex = 0;
                        for(size_t antenna1=0;antenna1!=antennaCount;++antenna1)
                        {
                                for(size_t antenna2=antenna1; antenna2!=antennaCount; ++antenna2)
                                {
                                        FlagMask* mask = _flagBuffers.find(std::make_pair(antenna1, antenna2))->second;
                                        size_t stride = mask->HorizontalStride();
                                        _flagReader->Read(); //Need to create
                                        ++baselineIndex;
                                }
                        }
                        _progressBar.reset();
         }

         std::string taskDescription;
         if(_rfiDetection)
         {
                        if(_collectStatistics)
                                taskDescription = "RFI detection, stats, conjugations, subband ordering and cable length corrections";
                        else
                                taskDescription = "RFI detection, conjugations, subband ordering and cable length corrections";
         }
         else
         {
                        if(_collectStatistics)
                                taskDescription = "Stats, conjugations, subband ordering and cable length corrections";
                        else
                                taskDescription = "Conjugations, subband ordering and cable length corrections";
         }
         _progressBar.reset(new ProgressBar(taskDescription));

         boost::thread_group threadGroup;
         for(size_t i=0; i!=_threadCount; ++i)
                        threadGroup.create_thread(boost::bind(&Rtflagger::baselineProcessThreadFunc, this));
                threadGroup.join_all();

         _progressBar.reset();
         _processWatch.Pause();


}




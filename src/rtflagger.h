#ifndef RTFLAGGER_H
#define RTFLAGGER_H

#include "stopwatch.h"

#include <boost/thread/mutex.hpp>

#include <vector>
#include <queue>
#include <set>
#include <string>

namespace aoflagger {
	class AOFlagger;
	class FlagMask;
	class ImageSet;
	class QualityStatistics;
	class Strategy;
}

struct SpeadRecData{
                int channel_index;
                double freq_start_hz;
                double freq_inc_hz;
                int num_baselines;
                int num_channels;
                int num_stations;
                int num_pols;
                double phase_centre_ra_deg;
                double phase_centre_dec_deg;
                double time_average_sec;
                int time_index;
                double time_inc_sec;
                double time_start_mjd_utc; 
                double *uu;
                double *vv;
                double *ww;
                double *vis_amp;
                };

class Rtflagger
{
	public:
		Rtflagger();
		~Rflagger();
                void Run(SpeadRecData SRecData);
                void SetFlagFileTemplate(const std::string& flagFileTemplate) { _flagFileTemplate = flagFileTemplate; }
		
	private:
		Rtflagger(const Rtflagger&) { }
                Stopwatch _readWatch, _processWatch, _writeWatch;
                aoflagger::AOFlagger *_flagger;
                aoflagger::Strategy *_strategy;
                std::map<std::pair<size_t, size_t>, aoflagger::ImageSet*> _imageSetBuffers;
                std::map<std::pair<size_t, size_t>, aoflagger::FlagMask*> _flagBuffers;
                aoflagger::FlagMask *_correlatorMask, *_fullysetMask;
                std::queue<std::pair<size_t,size_t> > _baselinesToProcess;
                std::unique_ptr<ProgressBar> _progressBar;
                size_t _baselinesToProcessCount;
		void operator=(const Rflagger&) { }
                double _initDurationToFlag, _endDurationToFlag;
                std::unique_ptr<class FlagReader> _flagReader;

                void baselineProcessThreadFunc();
                void processBaseline(size_t antenna1, size_t antenna2, aoflagger::QualityStatistics &statistics);
};

#endif


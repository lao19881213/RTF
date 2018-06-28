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
                double uu;
                double vv;
                double ww;
                double vis_amp;
                };

class Rtflagger
{
	public:
		Rtflagger();
		~Rflagger();
                void Run(SpeadRecData SRecData);
                void SetSubbandEdgeFlagWidth(double edgeFlagWidth) { _subbandEdgeFlagWidthKHz = edgeFlagWidth; }
                void processAllContiguousBands(size_t timeAvgFactor, size_t freqAvgFactor);
                void processOneContiguousBand(const std::string& outputFilename, size_t timeAvgFactor, size_t freqAvgFactor);
		
	private:
		Rtflagger(const Rtflagger&) { }
                Stopwatch _readWatch, _processWatch, _writeWatch;
                aoflagger::AOFlagger *_flagger;
                aoflagger::Strategy *_strategy;
		void operator=(const Rflagger&) { }
                double _subbandEdgeFlagWidthKHz;
                size_t _subbandEdgeFlagCount;
                size_t _subbandCount;
                size_t _quackInitSampleCount, _quackEndSampleCount;
                double _initDurationToFlag, _endDurationToFlag;
};

#endif


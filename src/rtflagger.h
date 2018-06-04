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

class Rtflagger
{
	public:
		Rtflagger();
		~Rflagger();
                void Run(double timeRes_s, double freqRes_kHz);
		
	private:
		Rtflagger(const Rtflagger&) { }
                Stopwatch _readWatch, _processWatch, _writeWatch;
                aoflagger::AOFlagger *_flagger;
                aoflagger::Strategy *_strategy;
		void operator=(const Rflagger&) { }
};

#endif


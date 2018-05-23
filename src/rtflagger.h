#ifndef RTFLAGGER_H
#define RTFLAGGER_H

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
		
	private:
		Rtflagger(const Rtflagger&) { }
		void operator=(const Rflagger&) { }
};

#endif


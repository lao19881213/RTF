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
        _readWatch.Start();

        _subbandEdgeFlagCount = round(_subbandEdgeFlagWidthKHz / (SRecData.freq_inc_hz/1000.0));
        _quackInitSampleCount = round(_initDurationToFlag / _mwaConfig.Header().integrationTime);
        _quackEndSampleCount = round(_endDurationToFlag / _mwaConfig.Header().integrationTime);
        _flagger = new AOFlagger();

        

}

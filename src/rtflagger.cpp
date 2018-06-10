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

void Rtflagger::Run(double timeRes_s, double freqRes_kHz, SpeadRecData SRecData)
{
        _readWatch.Start();

}

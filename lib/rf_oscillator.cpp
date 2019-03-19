# include "netxpto.h"
# include "rf_oscillator.h"

# include <algorithm>
# include <complex>
# include <iostream>
# include <fstream>
# include <random>

using namespace std;

void RfOscillator::initialize(void)
{
	outputSignals[0]->setSamplingPeriod(samplingPeriod);
	outputSignals[0]->setSymbolPeriod(symbolPeriod);

}


bool RfOscillator::runBlock(void)
{
	int space = outputSignals[0]->space();
	int process = space;

	if (process == 0) return false;

	double dt = outputSignals[0]->getSamplingPeriod();

	for (int i = 0; i < process; i++)
	{
		phase = phase + 2*PI*rfFrequency * dt;
		t_complex out = (rfAmplitude)*exp(t_complex(0,exponentialSign*phase));
		outputSignals[0]->bufferPut((t_complex)out);

	}

	return true;

}
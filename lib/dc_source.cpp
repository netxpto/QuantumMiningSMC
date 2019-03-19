# include "netxpto.h"
# include "dc_source.h"

# include <algorithm>
# include <complex>
# include <iostream>
# include <fstream>
# include <random>

using namespace std;

void Dc::initialize(void)
{
	outputSignals[0]->setSamplingPeriod(samplingPeriod);
	outputSignals[0]->setSymbolPeriod(symbolPeriod);

}


bool Dc::runBlock(void)
{
	int space = outputSignals[0]->space();
	int process = space;

	if (process == 0) return false;

	double value = getDcAmplitude();

	for (int i = 0; i < process; i++)
	{
		t_complex out = { value,0 };
		outputSignals[0]->bufferPut((t_complex)out);

	}

	return true;

}
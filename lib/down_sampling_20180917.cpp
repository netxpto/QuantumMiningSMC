# include "down_sampling_20180917.h"

# include <algorithm>
# include <complex>
# include <iostream>
# include <fstream>
# include <random>

using namespace std;


void DownSampling::initialize(void) {

	//outputSignals[0]->setSamplingPeriod(samplingPeriod);
	//outputSignals[0]->setSymbolPeriod(symbolPeriod);
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod() * downSamplingFactor);
	//outputSignals[0]->setSamplesPerSymbol((int)(inputSignals[0]->getSamplesPerSymbol()/ downSamplingFactor));
}


bool DownSampling::runBlock(void) {


	int ready = inputSignals[0]->ready();
	int space = downSamplingFactor * outputSignals[0]->space();

	int process = min(ready, space);

	if (process == 0) return false;
	
	//double samplingPeriod = outputSignals[0]->getSamplingPeriod();
	//int samplesPerSymbol = outputSignals[0]->getSamplesPerSymbol();
	
	//outputSignals[0]->setSamplesPerSymbol(samplesPerSymbol);


	for (auto k = 0; k != process; k++)
	{
		t_real var{0.0};
		inputSignals[0]->bufferGet(&var);

		if (samplesToSkip == 0)
		{
			outputSignals[0]->bufferPut(var);
			samplesToSkip = downSamplingFactor - 1;
		}
		else 
			samplesToSkip--;

	}

	return true;

}

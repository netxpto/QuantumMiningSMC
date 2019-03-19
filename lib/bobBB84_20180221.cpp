#include "bobBB84_20180221.h"

void bobBB84::initialize(void) {
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}

bool bobBB84::runBlock(void) {

	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();
	int space = outputSignals[0]->space();

	int ready = min(ready0, ready1);
	int process = min(ready, space);

	if (process <= 0) return false;
	
	for (int i = 0; i < process; i++)
	{
		t_binary inBasis;
		inputSignals[1]->bufferGet(&inBasis);
		t_real measurement;
		inputSignals[0]->bufferGet(&measurement);

		if ((measurement == 0.0) || (measurement == 1.0)) {
			outputSignals[0]->bufferPut((t_real)(inBasis));
		}
		else {
			outputSignals[0]->bufferPut((t_real)(measurement));
		}
		
	}
		

	return true;
}

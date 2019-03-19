#include "bobBB84_20180206.h"

void bobBB84::initialize(void) {

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	outputSignals[1]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[1]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[1]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
	outputSignals[1]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}

bool bobBB84::runBlock(void) {
	int ready = inputSignals[0]->ready();
	int space0 = outputSignals[0]->space();
	int space1 = outputSignals[1]->space();
	int space = min(space0, space1);

	int process = min(ready, space);

	if (process < 0) return false;

	for (auto k = 0; k < process; k++) {
		outputSignals[0]->bufferPut((t_real)1.0);
		outputSignals[1]->bufferPut((t_real)2.0);
	}
/*	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();
	int ready = min(ready0, ready1);

	int space0 = outputSignals[0]->space();
	int space1 = outputSignals[1]->space();
	int space = min(space0, space1);

	int process = min(ready, space);
	if (process < 0) return false;

	for (auto k = 0; k < process; k++) {
		t_real measurement;
		inputSignals[0]->bufferGet(&measurement);
		t_binary basisBob;
		inputSignals[1]->bufferGet(&basisBob);

		t_real output;
		
		if ((measurement == 0.0) || (measurement == 1.0)) {
			if (basisBob == 1)
				output = 1.0;
			else
				output = 0.0;
		}
		else
			output = measurement;

		outputSignals[0]->bufferPut((t_real)output);
		outputSignals[1]->bufferPut((t_real)output);
		cout << "k : " << k << "\n";
	}
*/	
	return true;
}

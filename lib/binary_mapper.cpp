#include "binary_mapper.h"
#include "netxpto.h"

void BinaryMapper::initialize(void) {
	outputSignals[0]->samplingPeriod = inputSignals[0]->samplingPeriod;
	outputSignals[0]->symbolPeriod = inputSignals[0]->symbolPeriod;
	outputSignals[0]->samplesPerSymbol = inputSignals[0]->samplesPerSymbol;
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}

bool BinaryMapper::runBlock(void) {
	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int length = min(space, ready);

	if (length <= 0) return false;

	int binValue;

	for (int i = 0; i <= length; i++) {
		inputSignals[0]->bufferGet(&binValue);

		if (binValue == 0)
			outputSignals[0]->bufferPut((t_real)low);
		else
			outputSignals[0]->bufferPut((t_real)high);
	}

	return true;
}
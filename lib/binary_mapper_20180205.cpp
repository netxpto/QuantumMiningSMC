#include "binary_mapper_20180205.h"


void BinaryMapper::initialize(void) {
	outputSignals[0]->setSamplingPeriod (inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSymbolPeriod (inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplesPerSymbol (inputSignals[0]->getSamplesPerSymbol());
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
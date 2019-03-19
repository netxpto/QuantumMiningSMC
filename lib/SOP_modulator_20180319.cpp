#include "SOP_modulator_20180319.h"

void SOPModulator::initialize(void) {
	int numberOfInputSignals = (int)inputSignals.size();

	if (numberOfInputSignals > 0) {
		outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
		outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
		outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
		outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	}
	else {
		outputSignals[0]->setSymbolPeriod(1);
		outputSignals[0]->setSamplingPeriod(1);
		outputSignals[0]->setSamplesPerSymbol(1);
		outputSignals[0]->setFirstValueToBeSaved(1);
	}
	
}

bool SOPModulator::runBlock(void) {
	bool alive{ false };

	int process{ 0 };
	int space0 = outputSignals[0]->space();
	int space1 = outputSignals[1]->space();
	int space = min(space0, space1);

	if (numberOfInputSignals > 0) {
		int ready = inputSignals[0]->ready(); //clock
		process = min(ready, space);
	}
	else
		process = space;

	if (process <= 0) return alive;

	else {
		for (auto k = 0; k < process; k++) {
			switch (sopType)
			{
			case Deterministic:
				outputSignals[0]->bufferPut((t_real)theta);
				outputSignals[1]->bufferPut((t_real)phi);

				alive = true;
				break;

			case Stocastic:
				break;

			default:
				break;
			}
		}
	}
	
	return alive;
}

#include "optical_attenuator_20180304.h"

void OpticalAttenuator::initialize(void) {
	
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}

bool OpticalAttenuator::runBlock(void) {
	bool alive = false;

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process <= 0) alive = false;

	else {
		double kappa = pow(10,(-kDB / 20));

		for (auto k = 0; k < process; k++) {
			t_complex_xy inValueXY;
			inputSignals[0]->bufferGet(&inValueXY);

			t_complex AinX = inValueXY.x;
			t_complex AinY = inValueXY.y;

			t_complex AoutX = AinX * kappa;
			t_complex AoutY = AinY * kappa;

			t_complex_xy outValueXY = { AoutX, AoutY };

			outputSignals[0]->bufferPut(outValueXY);
		}
		alive = true;
	}

	return alive;
}

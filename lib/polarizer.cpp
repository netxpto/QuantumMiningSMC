#include "netxpto.h"
#include "polarizer.h"

void Polarizer::initialize(void) {
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
}

bool Polarizer::runBlock(void) {
	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();

	int ready = min(ready0, ready1);

	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process <= 0) return false;

	for (auto k = 0; k < process; k++) {

		t_complex_xy valueXY;
		double tetha;

		inputSignals[0]->bufferGet(&valueXY);
		inputSignals[1]->bufferGet(&tetha);
		
		t_complex valueX = valueXY.x;
		t_complex valueY = valueXY.y;

		t_complex valueXout = cos(-tetha*PI / 180)*valueX + sin(-tetha*PI / 180)*valueY;
		t_complex ValueYout = -sin(-tetha*PI / 180)*valueX + cos(-tetha*PI / 180)*valueY;

		valueXY = { valueXout, ValueYout };

		outputSignals[0]->bufferPut(valueXY);

	}

	return true;
}


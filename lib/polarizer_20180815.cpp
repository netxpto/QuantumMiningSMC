# include "../include/polarizer_20180815.h"

void Polarizer::initialize(void) {
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}

bool Polarizer::runBlock(void) {

	auto ready0 = inputSignals[0]->ready();
	auto ready1 = inputSignals[1]->ready();
	auto ready = min(ready0, ready1);

	auto space = outputSignals[0]->space();
	auto process = min(ready, space);

	if (process <= 0) return false;

	for (auto k = 0; k != process; k++) {

		t_complex_xy valueXY;
		inputSignals[0]->bufferGet(&valueXY);

		t_real tetha;
		inputSignals[1]->bufferGet(&tetha);

		t_complex valueX = valueXY.x;
		t_complex valueY = valueXY.y;

		t_complex valueXout = cos(tetha)*valueX - sin(tetha)*valueY;
		t_complex ValueYout = sin(tetha)*valueX + cos(tetha)*valueY;
		
		t_complex_xy valueXYout = { valueXout, ValueYout };
		outputSignals[0]->bufferPut((t_complex_xy)valueXYout);

	}

	return true;
}


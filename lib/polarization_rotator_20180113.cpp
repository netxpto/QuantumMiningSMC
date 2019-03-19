#include "polarization_rotator_20180113.h"

void PolarizationRotator::initialize(void) {
	numberOfInputSignals = (int)inputSignals.size();

	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
}

bool PolarizationRotator::runBlock(void) {
	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();

	int ready = min(ready0, ready1);

	if (numberOfInputSignals == 3) 
		ready = min(ready, inputSignals[2]->ready());
	
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process <= 0) return false;

	for (auto k = 0; k < process; k++) {

		t_complex_xy valueXY;

		t_complex i (0,1);

		if (numberOfInputSignals == 3)
			inputSignals[2]->bufferGet(&phi);

		inputSignals[0]->bufferGet(&valueXY);
		inputSignals[1]->bufferGet(&tetha);

		t_complex valueX = valueXY.x;
		t_complex valueY = valueXY.y;

		t_complex valueXout = cos(tetha * PI / 180)*valueX + sin(tetha * PI / 180)*valueY*exp(-i*(phi*PI / 180));
		t_complex ValueYout = -sin(tetha * PI / 180)*valueX*exp(i* (phi*PI/180)) + cos(tetha * PI / 180)*valueY;
		t_complex_xy valueXYout = { valueXout, ValueYout };
		
		outputSignals[0]->bufferPut((t_complex_xy)valueXYout);

	}

	return true;
}


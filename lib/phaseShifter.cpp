#include <algorithm>
#include <complex>

#include "netxpto.h"
#include "phaseShifter.h"


void phaseShifter::initialize(void) {

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	outputSignals[0]->centralWavelength = outputOpticalWavelength;
	outputSignals[0]->centralFrequency = outputOpticalFrequency;

}


bool phaseShifter::runBlock(void) {
	int ready = inputSignals[0]->ready();

	int space1 = outputSignals[0]->space();
	int space2 = outputSignals[0]->space();
	int space = min(space1, space2);

	int process = min(ready, space);

	if (process == 0) return false;

	t_complex imaginary = (0, 1);

	for (int i = 0; i < process; i++) {

		t_complex input;
		inputSignals[0]->bufferGet(&input);

		t_complex out;

		out = input*imaginary;

		outputSignals[0]->bufferPut(out);


	}
	return true;
}
#include <algorithm>
#include <complex>
#include <iostream>
#include <fstream>

#include "netxpto_20180418.h"
#include "balanced_beam_splitter_20180124.h"


void BalancedBeamSplitter::initialize(void) {

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	outputSignals[0]->setCentralWavelength(inputSignals[0]->getCentralWavelength());
	outputSignals[0]->setCentralFrequency(inputSignals[0]->getCentralFrequency());

	outputSignals[1]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[1]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[1]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	outputSignals[1]->setCentralWavelength(inputSignals[0]->getCentralWavelength());
	outputSignals[1]->setCentralFrequency(inputSignals[0]->getCentralFrequency());

}

bool BalancedBeamSplitter::runBlock(void) {

	int ready1 = inputSignals[0]->ready();
	int ready2 = inputSignals[1]->ready();
	int ready = min(ready1, ready2);

	int space1 = outputSignals[0]->space();
	int space2 = outputSignals[1]->space();
	int space = min(space1, space2);

	int process = min(ready, space);

	if (process == 0) return false;
	

	for (int i = 0; i < process; i++) {

		if (mode = 0) {

			t_complex inSignal1;
			//This should implement a 1x2 beam splitter

				inputSignals[0]->bufferGet(&inSignal1);

				t_complex outSignal1 = matrix[0] * inSignal1;
				t_complex outSignal2 = matrix[2] * inSignal1;

				outputSignals[0]->bufferPut(outSignal1);
				outputSignals[1]->bufferPut(outSignal2);

			}

		else {

			t_complex inSignal1;
			t_complex inSignal2;
		
			complex<t_real> imaginary(0, 1);

			//This should implement a 2x2 beam splitter

				inputSignals[0]->bufferGet(&inSignal1);
				inputSignals[1]->bufferGet(&inSignal2);

				t_complex out1 = matrix[0] * inSignal1 + matrix[1] * inSignal2;
				t_complex out2 = matrix[2] * inSignal1 + matrix[3] * inSignal2;

				outputSignals[0]->bufferPut(out1);
				outputSignals[1]->bufferPut(out2);

			
		}
	

	}
	return true;
};

#include <algorithm>
#include <complex>
#include <chrono>
#include <fstream>

#include "netxpto.h"
#include "single_photo_detector.h"
#include <random>


void SinglePhotodiode::initialize(void) {

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}


bool SinglePhotodiode::runBlock(void) {

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();
	int process = min(ready, space);
	if (process == 0) return false;


		for (int i = 0; i < process; i++)
		{

			t_complex input;
			t_complex output;
			t_real out;

			inputSignals[0]->bufferGet(&input);

			output = abs(input)*abs(input);
			output = responsivity*output;

			out = real(output);

			outputSignals[0]->bufferPut(t_real(real(out))); // Taking real part only to apply the Kramers-Kronig algorithim
		}

	return true;
}

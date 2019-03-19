#include <algorithm>
#include <complex>
#include <iostream>
#include <fstream>

#include "netxpto.h"
#include "amplitude_modulator.h"


void AmplitudeModulator::initialize(void){

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	 
	outputSignals[0]->setCentralWavelength(inputSignals[0]->getCentralWavelength());
	outputSignals[0]->setCentralFrequency(inputSignals[0]->getCentralWavelength());

}


bool AmplitudeModulator::runBlock(void){
	
	int ready = inputSignals[0]->ready();

	int samplesPerSymbol = static_cast<int> (inputSignals[0]->getSamplesPerSymbol());

	int samplesHigh = static_cast<int> (dutyCycle * samplesPerSymbol);

	int space = outputSignals[0]->space();
	
	int process = min(ready, space);

	if (process == 0) return false;
	
	complex<t_real> imaginary(0, 1);
	
	for (int i = 0; i < process; i++) {

		if (aux == samplesPerSymbol) aux = 1;

		t_complex in;
		t_complex out;

		inputSignals[0]->bufferGet(&in);

		out = in;

		if (aux>samplesHigh)
		{
			out = 0;
		}


		outputSignals[0]->bufferPut(out);
	
		aux++;

	}
	return true;
}

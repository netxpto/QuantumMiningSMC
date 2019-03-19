#include <algorithm>
#include <complex>
#include <iostream>

#include "netxpto.h"
#include "delayer.h"



void Delayer::initialize(void){

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}


bool Delayer::runBlock(void){
	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();


	int process = min(ready, space);

	t_real out;

	if (process == 0) return false;




	
	for (int i = 0; i < process; i++) {

		t_real signalValue;
		inputSignals[0]->bufferGet(&signalValue);
		auxint = auxint + 1;
		if (auxint >= delay)
		{
			out = signalValue;
			outputSignals[0]->bufferPut(out);
		}

	}

	return true;
}
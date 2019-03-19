#include <algorithm>
#include <complex>

#include "netxpto.h"
#include "bit_decider.h"


void BitDecider::initialize(void){

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());

}


bool BitDecider::runBlock(void){
	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process == 0) return false;

	for (int k = 0; k < process; k++) {
		t_real in;
		inputSignals[0]->bufferGet(&in);
		if (in>decisionLevel){
			outputSignals[0]->bufferPut((t_binary) 1);
		}
		else {
			outputSignals[0]->bufferPut((t_binary) 0);
		}
	}
	return true;
	
}
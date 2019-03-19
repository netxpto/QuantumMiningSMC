#include <algorithm>
#include <complex>
#include <array>
#include <vector>

#include "netxpto_20180418.h"
#include "iir_filter_20180718.h"

using namespace std;

void IIR_Filter::initialize(void) {

	double samplingPeriod = inputSignals[0]->getSamplingPeriod();
	double symbolPeriod = inputSignals[0]->getSymbolPeriod();
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());

};


bool IIR_Filter::runBlock(void) {

	int ready;
	int space;
	double inValue;
	double outValue;

	if (firstTime){
		
	// Resize state vectors to required size
		currState.resize(getFilterOrder()-1, 0); 
		oldState.resize(getFilterOrder()-1, 0); 

	// Pad the smallest coefficient vectors with zeros
		if (aCoeff.size() > bCoeff.size()) {
			bCoeff.resize(aCoeff.size(),0);
		} else if (bCoeff.size() > aCoeff.size()) {
			aCoeff.resize(bCoeff.size(),0);
		}
	}

	ready = inputSignals[0]->ready();
	space = outputSignals[0]->space();

	int available = min(ready, space);

	if (available == 0) {
		return false;
	}

	inputSignals[0]->bufferGet(&inValue);

	bool running = true;

	oldState.assign(currState.begin(), currState.end());
	outValue = bCoeff[0]*inValue+currState[0];

	for (int i = 0; i < currState.size(); i++) {
		if (i==currState.size()-1) {
			currState[i] = bCoeff[i+1]*inValue-aCoeff[i+1]*outValue;
		} else {
			currState[i] = oldState[i+1]+bCoeff[i+1]*inValue-aCoeff[i+1]*outValue;
		}
	}
	if (count < samplesToSkip) { count += 1; }
	
	if (count >= samplesToSkip){ outputSignals[0]->bufferPut(outValue); }


	return true;
}

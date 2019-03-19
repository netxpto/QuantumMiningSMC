#include "discrete_optical_amplifier.h"


DiscreteOpticalAmplifier::DiscreteOpticalAmplifier(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal) :SuperBlock(inputSignal, outputSignal) {

	inputSignals = inputSignal;
	outputSignals = outputSignal;

	B1.initializeBlock(vector<Signal*> { inputSignals[0] }, vector<Signal*> { &EDFA01 });
	B2.initializeBlock(vector<Signal*> { }, vector<Signal*> { &EDFA02 });
	B3.initializeBlock(vector<Signal*> { &EDFA01, &EDFA02 }, vector<Signal*> { &EDFA03 });
	
	setModuleBlocks({ &B1, &B2, &B3 });
};


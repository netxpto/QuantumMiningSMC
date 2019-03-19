#include "i_homodyne_receiver_20180124.h"


I_HomodyneReceiver::I_HomodyneReceiver(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal) :SuperBlock(inputSignal, outputSignal) {

	inputSignals = inputSignal;
	outputSignals = outputSignal;

	B1.initializeBlock(vector<Signal*> { inputSignals[0], inputSignals[1] }, vector<Signal*> { &I_HMD01 });
	B2.initializeBlock(vector<Signal*> { &I_HMD01 }, vector<Signal*> { &I_HMD02 });
	B3.initializeBlock(vector<Signal*> { }, vector<Signal*> { &I_HMD03 });
	B4.initializeBlock(vector<Signal*> { &I_HMD02, &I_HMD03 }, vector<Signal*> { &I_HMD04 });

	setModuleBlocks({ &B1, &B2, &B3, &B4 });
};


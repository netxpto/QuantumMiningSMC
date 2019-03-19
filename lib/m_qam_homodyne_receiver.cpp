#include "homodyne_receiver.h"

HomodyneReceiver::HomodyneReceiver(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal) :SuperBlock(inputSignal, outputSignal) {

	setLocalOscillatorSamplingPeriod(inputSignal[0]->getSamplingPeriod());

	inputSignals = inputSignal;
	outputSignals = outputSignal;

	B1.initializeBlock(vector<Signal*> { }, vector<Signal*> { &HMD01 }); //Local oscillator
	B2.initializeBlock(vector<Signal*> { inputSignals[0], &HMD01 }, vector<Signal*> { &HMD02, &HMD03, &HMD04, &HMD05 }); //Optical hybrid
	B3.initializeBlock(vector<Signal*> { &HMD02, &HMD03 }, vector<Signal*> { &HMD06 }); //Photodiodes
	B4.initializeBlock(vector<Signal*> { &HMD04, &HMD05 }, vector<Signal*> { &HMD07 }); //Photodiodes
	B5.initializeBlock(vector<Signal*> { &HMD06 }, vector<Signal*> { &HMD08 }); //Amplifier
	B6.initializeBlock(vector<Signal*> { &HMD07 }, vector<Signal*> { &HMD09 }); //Amplifier
	B7.initializeBlock(vector<Signal*> { &HMD08 }, vector<Signal*> { &HMD10 }); //Filter
	B8.initializeBlock(vector<Signal*> { &HMD09 }, vector<Signal*> { &HMD11 }); //Filter
	B9A.initializeBlock(vector<Signal*> { }, vector<Signal*> { &HMD12A }); //Clock
	B9B.initializeBlock(vector<Signal*> { }, vector<Signal*> { &HMD12B }); //Clock
	B10.initializeBlock(vector<Signal*> { &HMD10, &HMD12A }, vector<Signal*> { &HMD13 }); //Sampler
	B11.initializeBlock(vector<Signal*> { &HMD11, &HMD12B }, vector<Signal*> { &HMD14 }); //Sampler
	B12.initializeBlock(vector<Signal*> { &HMD13, &HMD14 }, vector<Signal*> { &HMD15 }); //Decoder

	setModuleBlocks({ &B1, &B2, &B3, &B4, &B5, &B6, &B7, &B8, &B9A, &B9B, &B10, &B11, &B12 });
	
};

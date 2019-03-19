#include "homodyne_receiver_20171203.h"

HomodyneReceiver::HomodyneReceiver(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal) :SuperBlock(inputSignal, outputSignal) {

	setLocalOscillatorSamplingPeriod(inputSignal[0]->getSamplingPeriod());

	inputSignals = inputSignal;
	outputSignals = outputSignal;

/*
	B1.initializeBlock(vector<Signal*> { }, vector<Signal*> { &HMD01 }); //Local oscillator
	B2.initializeBlock(vector<Signal*> { inputSignals[0], &HMD01 }, vector<Signal*> { &HMD02, &HMD03, &HMD04, &HMD05 }); //Optical hybrid
	B3.initializeBlock(vector<Signal*> { &HMD02, &HMD03 }, vector<Signal*> { &HMD06 }); //Photodiodes
	B4.initializeBlock(vector<Signal*> { &HMD04, &HMD05 }, vector<Signal*> { &HMD07 }); //Photodiodes
	B5.initializeBlock(vector<Signal*> { &HMD06 }, vector<Signal*> { &HMD08 }); //Amplifier
	B6.initializeBlock(vector<Signal*> { &HMD07 }, vector<Signal*> { &HMD09 }); //Amplifier
	B7.initializeBlock(vector<Signal*> { &HMD08 }, vector<Signal*> { &HMD10 }); //Filter
	B8.initializeBlock(vector<Signal*> { &HMD09 }, vector<Signal*> { &HMD11 }); //Filter
	B9.initializeBlock(vector<Signal*> { }, vector<Signal*> { &HMD12 }); //Clock
	B10.initializeBlock(vector<Signal*> { &HMD10 }, vector<Signal*> { &HMD13 }); //Sampler
	B11.initializeBlock(vector<Signal*> { &HMD11 }, vector<Signal*> { &HMD14 }); //Sampler
	B12.initializeBlock(vector<Signal*> { &HMD13, &HMD14 }, vector<Signal*> { &HMD15 }); //Decoder
	*/

	B1.initializeBlock(vector<Signal*> { }, vector<Signal*> { &HMD01 }); //Local oscillator
	B2.initializeBlock(vector<Signal*> { inputSignals[0], &HMD01 }, vector<Signal*> { &HMD02, &HMD03, &HMD04, &HMD05 }); //Optical hybrid
	B3.initializeBlock(vector<Signal*> { &HMD02, &HMD03 }, vector<Signal*> { &HMD06 }); //Photodiodes
	B4.initializeBlock(vector<Signal*> { &HMD04, &HMD05 }, vector<Signal*> { &HMD07 }); //Photodiodes
	B5.initializeBlock(vector<Signal*> { &HMD06 }, vector<Signal*> { &HMD08 }); //Amplifier
	B6.initializeBlock(vector<Signal*> { &HMD07 }, vector<Signal*> { &HMD09 }); //Amplifier
	B7.initializeBlock(vector<Signal*> { }, vector<Signal*> { &HMD10 }); //Noise
	B8.initializeBlock(vector<Signal*> { }, vector<Signal*> { &HMD11 }); //Noise
	B9.initializeBlock(vector<Signal*> { &HMD08, &HMD10 }, vector<Signal*> { &HMD12 }); //Add
	B10.initializeBlock(vector<Signal*> { &HMD09, &HMD11 }, vector<Signal*> { &HMD13 }); //Add
	B11.initializeBlock(vector<Signal*> { &HMD12 }, vector<Signal*> { &HMD14 }); //Pulse Shaper
	B12.initializeBlock(vector<Signal*> { &HMD13 }, vector<Signal*> { &HMD15 }); //Pulse Shaper
	B13.initializeBlock(vector<Signal*> { &HMD14 }, vector<Signal*> { &HMD16 }); //Sampler
	B14.initializeBlock(vector<Signal*> { &HMD15 }, vector<Signal*> { &HMD17 }); //Sampler
	B15.initializeBlock(vector<Signal*> { &HMD16, &HMD17 }, vector<Signal*> { &HMD18 }); //Decoder

//	setModuleBlocks({ &B1, &B2, &B3, &B4, &B5, &B6, &B7, &B8, &B9, &B10, &B11, &B12 });
	setModuleBlocks({ &B1, &B2, &B3, &B4, &B5, &B6, &B7, &B8, &B9, &B10, &B11, &B12, &B13, &B14, &B15 });
//	setModuleBlocks({ &B1, &B2, &B3, &B4, &B5, &B6, &B7, &B8, &B9, &B10, &B13, &B14, &B15 });
};

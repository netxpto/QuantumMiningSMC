#include "homodyne_receiver_20181213.h"

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


	B7.initializeBlock(vector<Signal*> { }, vector<Signal*> { &HMD10 }); //Noise
	B8.initializeBlock(vector<Signal*> { }, vector<Signal*> { &HMD11 }); //Noise

	B9.initializeBlock(vector<Signal*> { &HMD08, &HMD10 }, vector<Signal*> { &HMD12 }); //Add
	B10.initializeBlock(vector<Signal*> { &HMD09, &HMD11 }, vector<Signal*> { &HMD13 }); //Add
//
	B11.initializeBlock(vector<Signal*> { &HMD12 }, vector<Signal*> { &HMD14 }); //Electrical Filter
	B12.initializeBlock(vector<Signal*> { &HMD13 }, vector<Signal*> { &HMD15 }); //Electrical Filter

	B13.initializeBlock(vector<Signal*> { }, vector<Signal*> { &HMD16 }); //Noise
	B14.initializeBlock(vector<Signal*> { }, vector<Signal*> { &HMD17 }); //Noise
	
	B15.initializeBlock(vector<Signal*> { &HMD14, &HMD16}, vector<Signal*> { &HMD18 }); //Add
	B16.initializeBlock(vector<Signal*> { &HMD15, &HMD17 }, vector<Signal*> { &HMD19 }); //Add

	B17.initializeBlock(vector<Signal*> { &HMD18, &HMD19 }, vector<Signal*> { &HMD20, &HMD21 }); // Power Spectral Density

	B18.initializeBlock(vector<Signal*> { &HMD20, &HMD21}, vector<Signal*> { &HMD22, &HMD23}); //SNR estimation

	B19.initializeBlock(vector<Signal*> { &HMD22}, vector<Signal*> { &HMD24 }); //Pulse Shaper
	B20.initializeBlock(vector<Signal*> { &HMD23 }, vector<Signal*> { &HMD25 }); //Pulse Shaper
	
	B21.initializeBlock(vector<Signal*> { &HMD24, &HMD25 }, vector<Signal*> { &HMD26, &HMD27}); //SNR estimation
	
	B22.initializeBlock(vector<Signal*> { &HMD26 }, vector<Signal*> { &HMD28 }); //Sampler
	B23.initializeBlock(vector<Signal*> { &HMD27 }, vector<Signal*> { &HMD29 }); //Sampler
	
	B24.initializeBlock(vector<Signal*> { &HMD28, &HMD29 }, vector<Signal*> { &HMD30 }); //Decoder

//	setModuleBlocks({ &B1, &B2, &B3, &B4, &B5, &B6, &B7, &B8, &B9, &B10, &B11, &B12 });
//	setModuleBlocks({ &B1, &B2, &B3, &B4, &B5, &B6, &B7, &B8, &B9, &B10, &T1, &B11, &B12, &T2, &B13, &B14, &B15 });
//	setModuleBlocks({ &B1, &B2, &B3, &B4, &B5, &B6,&n1,&n2,&n3,&n4,&n5,&n6, &B7, &B8, &B9, &B10, &B11,&B12, &B13, &B14, &B15, &B16, &B17, &B18, &B19, &B20, &B21, &B22, &B23, &B24 });
	setModuleBlocks({ &B1, &B2, &B3, &B4, &B5, &B6, &B7, &B8, &B9, &B10, &B11,&B12, &B13, &B14, &B15, &B16, &B17, &B18, &B19, &B20, &B21, &B22, &B23, &B24 });
//	setModuleBlocks({ &B1, &B2, &B3, &B4, &B5, &B6, &B7, &B8, &B9, &B10, &T1, &B13, &B14, &B15 });
//	setModuleBlocks({ &B1, &B2, &B3, &B4, &B5, &B6, &B7, &B8, &B9, &B10, &B13, &B14, &B15 });
};

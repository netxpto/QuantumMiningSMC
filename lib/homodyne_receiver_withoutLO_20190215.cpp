#include "homodyne_receiver_withoutLO_20190215.h"

HomodyneReceiverWithoutLO::HomodyneReceiverWithoutLO(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal) :SuperBlock(inputSignal, outputSignal) {

	inputSignals = inputSignal;
	outputSignals = outputSignal;

	B01.initializeBlock(vector<Signal*> { inputSignals[0], inputSignals[1] }, vector<Signal*> { &HMD01, &HMD02, &HMD03, &HMD04 }); //Optical hybrid
	
	B02.initializeBlock(vector<Signal*> { &HMD01, &HMD02 }, vector<Signal*> { &HMD05 }); //Photodiodes
	B03.initializeBlock(vector<Signal*> { &HMD03, &HMD04 }, vector<Signal*> { &HMD06 }); //Photodiodes
	
//	TI_Amplifier B04{ vector<Signal*> { &HMD05 }, vector<Signal*> { &HMD07 } };
//	TI_Amplifier B05{ vector<Signal*> { &HMD06 }, vector<Signal*> { &HMD08 } };
//
//	B04 and B05 defined and initialized in header file. TI_Amplifier is a SuperBlock
//	and apparently does not work with initializeBlock. Currently there is no
//	initializeSuperBlock method. So it is called on declaration, as I currently
//	don't have time to look for another way, and this one apparently works.

	B04.setSaveInternalSignals(true);
	B05.setSaveInternalSignals(true);
	
	B06.initializeBlock(vector<Signal*> { &HMD07}, vector<Signal*> { &HMD09 }); //Pulse Shaper
	B07.initializeBlock(vector<Signal*> { &HMD08 }, vector<Signal*> { &HMD10 }); //Pulse Shaper

	B08.initializeBlock(vector<Signal*> { }, vector<Signal*> { &HMD11 }); //Noise
	B09.initializeBlock(vector<Signal*> { }, vector<Signal*> { &HMD12 }); //Noise
	
	B10.initializeBlock(vector<Signal*> { &HMD09, &HMD11}, vector<Signal*> { &HMD13 }); //Add
	B11.initializeBlock(vector<Signal*> { &HMD10, &HMD12 }, vector<Signal*> { &HMD14 }); //Add

	B12.initializeBlock(vector<Signal*> { &HMD13, &HMD14 }, vector<Signal*> { &HMD15, &HMD16 }); // Power Spectral Density
	B13.initializeBlock(vector<Signal*> { &HMD15, &HMD16 }, vector<Signal*> { &HMD17, &HMD18}); //SNR estimation
	
	B14.initializeBlock(vector<Signal*> { &HMD17 }, vector<Signal*> { &HMD19 }); //Sampler
	B15.initializeBlock(vector<Signal*> { &HMD18 }, vector<Signal*> { &HMD20 }); //Sampler
	
	B16.initializeBlock(vector<Signal*> { &HMD19, &HMD20 }, vector<Signal*> { &HMD21 }); //Decoder

	setModuleBlocks({ &B01, &B02, &B03, &B04, &B05,&B06,&B07,&B08,&B09,&B10,&B11,&B12, &B13, &B14, &B15, &B16 });
};

void HomodyneReceiverWithoutLO::setDirName(string newDirName) {
	dirName = newDirName;
	HMD01.setFolderName(dirName);
	HMD02.setFolderName(dirName);
	HMD03.setFolderName(dirName);
	HMD04.setFolderName(dirName);
	HMD05.setFolderName(dirName);
	HMD06.setFolderName(dirName);
	HMD07.setFolderName(dirName);
	HMD08.setFolderName(dirName);
	HMD09.setFolderName(dirName);
	HMD10.setFolderName(dirName);
	HMD11.setFolderName(dirName);
	HMD12.setFolderName(dirName);
	HMD13.setFolderName(dirName);
	HMD14.setFolderName(dirName);
	HMD15.setFolderName(dirName);
	HMD16.setFolderName(dirName);
	HMD17.setFolderName(dirName);
	HMD18.setFolderName(dirName);
	HMD19.setFolderName(dirName);
	HMD20.setFolderName(dirName);
	HMD21.setFolderName(dirName);
	B04.setDirName(dirName);
	B05.setDirName(dirName);
}
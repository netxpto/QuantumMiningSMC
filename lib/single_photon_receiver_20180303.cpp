#include "single_photon_receiver_20180303.h"

SinglePhotonReceiver::SinglePhotonReceiver(vector<Signal*> &inputSignal, vector <Signal*> &outputSignal) : SuperBlock(inputSignal, outputSignal) {

	

	B1.initializeBlock(vector<Signal*>{ inputSignals[0] }, vector<Signal*>{ &SPR01 }); //Pulse shapper

	B2.initializeBlock(vector<Signal*>{ inputSignals[1] }, vector<Signal*>{ &SPR02 }); //Pulse shapper

	B3.initializeBlock(vector<Signal*>{}, vector<Signal*>{ &SPR03 });					//Clock

	B4.initializeBlock(vector<Signal*>{}, vector<Signal*>{ &SPR04 });					//Clock
	
	B5.initializeBlock(vector<Signal*>{ &SPR01, &SPR03 }, vector<Signal*>{ &SPR05 });	//Sampler

	B6.initializeBlock(vector<Signal*>{ &SPR02, &SPR04 }, vector<Signal*>{ &SPR06 });	//Sampler

	B7.initializeBlock(vector<Signal*>{ &SPR05 }, vector<Signal*>{ &SPR07 });			//Bit Decider

	B8.initializeBlock(vector<Signal*>{ &SPR06 }, vector<Signal*>{ &SPR08 });			//Bit Decider

	B9.initializeBlock(vector<Signal*>{ &SPR07, &SPR08 }, vector<Signal*>{ &SPR09 });		//Coincidence Detector
	
	B10.initializeBlock(vector<Signal*>{ &SPR09 }, vector<Signal*>{ &SPR09_out });


	setModuleBlocks({ &B1, &B2, &B3, &B4, &B5, &B6, &B7, &B8, &B9, &B10 });
};


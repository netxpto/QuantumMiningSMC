#include "adc_20180423.h"

ADC::ADC(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal) :SuperBlock(inputSignal, outputSignal) {

	setQuantizerSamplingPeriod(inputSignal[0]->getSamplingPeriod());

	inputSignals = inputSignal;
	outputSignals = outputSignal;

/*

	*/

	B01.initializeBlock(vector<Signal*> { inputSignals[0] }, vector<Signal*> { &ADC01 }); //Resample
	//B02.initializeBlock(vector<Signal*> { inputSignals[1] }, vector<Signal*> { &ADC02 }); //Resample
	B02.initializeBlock(vector<Signal*> { &ADC01 }, vector<Signal*> { &ADC02 }); //Quntizer
	//B04.initializeBlock(vector<Signal*> { &ADC02 }, vector<Signal*> { &ADC04 }); //Quntizer
	
	setModuleBlocks({ &B01, &B02 });
};

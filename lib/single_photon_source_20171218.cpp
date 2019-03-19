# include "single_photon_source_20171218.h"

void SinglePhotonSource::initialize(void) {

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
//	outputSignals[0]->setCentralWavelength(outputOpticalWavelength);
//	outputSignals[0]->setCentralFrequency(outputOpticalFrequency);

}

bool SinglePhotonSource::runBlock(void) {

	

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int length = min(ready, space);

	if (length <= 0) return false;

	t_real re;

	for (int i = 0; i < length; i++) {
		
		inputSignals[0]->bufferGet(&re);

		t_complex valueX(re,0.0);
		
		t_complex valueY(0.0, 0.0);

		t_complex_xy valueXY = {valueX, valueY};

		outputSignals[0]->bufferPut((t_complex_xy) valueXY);
		
	}
			
	return true;

};

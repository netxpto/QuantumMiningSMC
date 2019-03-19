# include "../include/single_photon_source_20180815.h"

void SinglePhotonSource::initialize(void) {
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}

bool SinglePhotonSource::runBlock(void) {

	auto ready = inputSignals[0]->ready();
	auto space = outputSignals[0]->space();

	auto process = min(ready, space);

	if (process <= 0) return false;

	for (auto k = 0; k!=process; k++) {

		t_real re;
		inputSignals[0]->bufferGet(&re);
		t_complex_xy valueXY{ { re, 0.0 },{ 0.0, 0.0 } };
		outputSignals[0]->bufferPut((t_complex_xy)valueXY);
		
	}
			
	return true;

};

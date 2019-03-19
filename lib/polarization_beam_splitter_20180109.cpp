# include "polarization_beam_splitter_20180109.h"

void PolarizationBeamSplitter::initialize(void) {

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}

bool PolarizationBeamSplitter::runBlock(void) {

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process <= 0) return false;

	for (auto i = 0; i < process; i++) {

		t_complex_xy inSignal1;
		t_photon_mp_xy outSignal1;
		//This should implement a 1x2 beam splitters

		inputSignals[0]->bufferGet(&inSignal1);

		t_complex xValueH = (inSignal1.x * matrixH[0] * (1 / sqrt(pow(abs(inSignal1.x), 2) + pow(abs(inSignal1.y), 2)))) + (inSignal1.y * matrixH[1] * (1 / sqrt(pow(abs(inSignal1.x), 2) + pow(abs(inSignal1.y), 2))));
		t_complex yValueH = (inSignal1.x * matrixH[2] * (1 / sqrt(pow(abs(inSignal1.x), 2) + pow(abs(inSignal1.y), 2)))) + (inSignal1.y * matrixH[3] * (1 / sqrt(pow(abs(inSignal1.x), 2) + pow(abs(inSignal1.y), 2))));

		t_complex xValueV = (inSignal1.x * matrixV[0] * (1 / sqrt(pow(abs(inSignal1.x), 2) + pow(abs(inSignal1.y), 2)))) + (inSignal1.y * matrixV[1] * (1 / sqrt(pow(abs(inSignal1.x), 2) + pow(abs(inSignal1.y), 2))));
		t_complex yValueV = (inSignal1.x * matrixV[2] * (1 / sqrt(pow(abs(inSignal1.x), 2) + pow(abs(inSignal1.y), 2)))) + (inSignal1.y * matrixV[3] * (1 / sqrt(pow(abs(inSignal1.x), 2) + pow(abs(inSignal1.y), 2))));

		outSignal1.path[0] = { xValueH, yValueH };

		outSignal1.path[1] = { xValueV, yValueV };


		outputSignals[0]->bufferPut((t_photon_mp_xy) outSignal1);
	}
	
	return true;

};

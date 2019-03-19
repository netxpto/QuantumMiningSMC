# include "../include/polarization_beam_splitter_20181113.h"

void PolarizationBeamSplitter::initialize(void) {
	for (auto k : outputSignals)
	{
		k->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
		k->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
		k->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	}
}

bool PolarizationBeamSplitter::runBlock(void) {

	int ready = inputSignals[0]->ready();

	auto space = MAX_BUFFER_LENGTH;
	for (auto k : outputSignals) space = min(space, k->space());

	int process = min(ready, space);

	if (process <= 0) return false;

	for (auto i = 0; i < process; i++) {

		t_complex_xy inSignal1;
		t_photon_mp_xy outSignal1;
		//This should implement a 1x2 beam splitters

		inputSignals[0]->bufferGet(&inSignal1);
		/*
		t_complex xValueH = (inSignal1.x * matrixH[0] * (1 / sqrt(2) + pow(abs(inSignal1.y), 2))) + (inSignal1.y * matrixH[1] * (1 / sqrt(2) + pow(abs(inSignal1.y), 2)));
		t_complex yValueH = (inSignal1.x * matrixH[2] * (1 / sqrt(2) + pow(abs(inSignal1.y), 2))) + (inSignal1.y * matrixH[3] * (1 / sqrt(2) + pow(abs(inSignal1.y), 2)));

		t_complex xValueV = (inSignal1.x * matrixV[0] * (1 / sqrt(2) + pow(abs(inSignal1.y), 2))) + (inSignal1.y * matrixV[1] * (1 / sqrt(2) + pow(abs(inSignal1.y), 2)));
		t_complex yValueV = (inSignal1.x * matrixV[2] * (1 / sqrt(2) + pow(abs(inSignal1.y), 2))) + (inSignal1.y * matrixV[3] * (1 / sqrt(2) + pow(abs(inSignal1.y), 2)));
		*/

		ah = inSignal1.x;
		av = inSignal1.y;
		array <t_complex, 4> inputs = { { ah, bh, av, bv } };

		t_complex ch = inputs[0] * pbs[0][0] + inputs[1] * pbs[0][1] + inputs[2] * pbs[0][2] + inputs[3] * pbs[0][3];
		t_complex dh = inputs[0] * pbs[1][0] + inputs[1] * pbs[1][1] + inputs[2] * pbs[1][2] + inputs[3] * pbs[1][3];
		t_complex cv = inputs[0] * pbs[2][0] + inputs[1] * pbs[2][1] + inputs[2] * pbs[2][2] + inputs[3] * pbs[2][3];
		t_complex dv = inputs[0] * pbs[3][0] + inputs[1] * pbs[3][1] + inputs[2] * pbs[3][2] + inputs[3] * pbs[3][3];

		outSignal1.path[0] = { ch, 0 };

		outSignal1.path[1] = { 0, dv };

		outputSignals[0]->bufferPut((t_photon_mp_xy)outSignal1);
	}

	return true;

};

# include "clock_20180815.h"

void Clock::initialize(void) {

	int numberOfSamplesPerSymbol = (int) round(period / samplingPeriod);

	for (auto k : outputSignals) {
		k->setSamplingPeriod(samplingPeriod);
		k->setSamplesPerSymbol(numberOfSamplesPerSymbol);
	}

	phaseShift = (int) round(phase*numberOfSamplesPerSymbol / (2 * PI));

};

bool Clock::runBlock(void) {

	int space = outputSignals[0]->space();
	for (auto k : outputSignals) {
		int aux = k->space();
		space = std::min(space, aux);
	}

	if (space == 0) return false;

	int numberOfSamplesPerSymbol = (int) outputSignals[0]->getSamplesPerSymbol();

	if (index != phaseShift) {
		int aux{0};
		
		for (int i = index; (i < phaseShift) & (space>0); i++) {
			for (auto k: outputSignals) k->bufferPut(0.0);
			space--;
			aux++;
		};
		index = index + aux;
		index = index % numberOfSamplesPerSymbol;
	};

	for (int k = 0; k < space; k++) {
		if (index == phaseShift) {
			for (auto k : outputSignals) k->bufferPut((t_real) 1.0);
		}
		else {
			for (auto k : outputSignals) k->bufferPut((t_real) 0.0);
		}
		index++;
		index = index % numberOfSamplesPerSymbol;
	}

	return true;
};
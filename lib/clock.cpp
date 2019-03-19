# include "netxpto.h"
# include "clock.h"

void Clock::initialize(void) {

	outputSignals[0]->setSamplingPeriod(samplingPeriod);

	int numberOfSamplesPerSymbol = (int) round(period / samplingPeriod);

	outputSignals[0]->setSamplesPerSymbol(numberOfSamplesPerSymbol);

	phaseShift = (int) round(phase*numberOfSamplesPerSymbol / (2 * PI));

};

bool Clock::runBlock(void) {

	int space = outputSignals[0]->space();

	if (space == 0) return false;

	int numberOfSamplesPerSymbol = (int) outputSignals[0]->getSamplesPerSymbol();

	if (index != phaseShift) {
		int aux{0};
		
		for (int i = index; (i < phaseShift) & (space>0); i++) {
			outputSignals[0]->bufferPut(0.0);
			space--;
			aux++;
		};
		index = index + aux;
		index = index % numberOfSamplesPerSymbol;
	};

	for (int k = 0; k < space; k++) {
		if (index == phaseShift) {
			outputSignals[0]->bufferPut((t_real) 1.0);
		}
		else {
			outputSignals[0]->bufferPut((t_real) 0.0);
		}
		index++;
		index = index % numberOfSamplesPerSymbol;
	}

	return true;
};
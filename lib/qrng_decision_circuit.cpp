# include <algorithm> // min()

# include "qrng_decision_circuit.h"

void QRNG_DecisionCircuit::initialize(void) {

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}

bool QRNG_DecisionCircuit::runBlock(void) {

	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();
	int space = outputSignals[0]->space();

	int ready = min(ready0, ready1);
	int length = min(ready, space);

	if (length <= 0) return false;

	for (int i = 0; i < length; i++) {
		t_real value0;
		inputSignals[0]->bufferGet(&value0);

		t_real value1;
		inputSignals[1]->bufferGet(&value1);

		if ((value0 == 1.0) && (value1 == 0.0)) {
			outputSignals[0]->bufferPut((t_binary)0);
		}

		if ((value0 == 0.0) && (value1 == 1.0)) {
			outputSignals[0]->bufferPut((t_binary)1);
		}
	}

	return true;

};

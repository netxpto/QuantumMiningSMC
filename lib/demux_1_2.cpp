#include "netxpto.h"
#include "demux_1_2.h"

void Demux_1_2::initialize(void) {
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());

	outputSignals[1]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	outputSignals[1]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[1]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
	outputSignals[1]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
}

bool Demux_1_2::runBlock(void) {
	int ready1 = inputSignals[0]->ready();
	int ready2 = inputSignals[1]->ready();

	int ready = min(ready1, ready2);

	int space1 = outputSignals[0]->space();
	int space2 = outputSignals[1]->space();

	int space = min(space1, space2);

	int length = min(ready, space);

	if (length <= 0) return false;

	signal_value_type inSignalType1 = inputSignals[0]->getValueType();
	signal_value_type inSignalType2 = inputSignals[1]->getValueType();

	if((inSignalType1 != BinaryValue) || (inSignalType2 != BinaryValue)){ 
		cout << "ERRO: discrete_to_continuous_time.cpp - invalide signal type" << "\n"; 
		return false;
	}

	int binValue;
	int logicalValue;

	for (int i = 0; i <= length; i++) {
		inputSignals[0]->bufferGet(&binValue);
		inputSignals[1]->bufferGet(&logicalValue);

		if (logicalValue == 1) {
			outputSignals[0]->bufferPut((t_binary)binValue);
		}
		else
			outputSignals[1]->bufferPut((t_binary)binValue);
	}

	return true;
}
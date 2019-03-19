#include "demux_1_2_20180205.h"

void Demux_1_2::initialize(void) {
//	outputSignals[0]->setFirstValueToBeSaved(inputSignals[1]->getFirstValueToBeSaved());
	outputSignals[0]->setSamplingPeriod(symbolPeriod);
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
	outputSignals[0]->setSymbolPeriod(symbolPeriod);

//	outputSignals[1]->setFirstValueToBeSaved(inputSignals[1]->getFirstValueToBeSaved());
	outputSignals[1]->setSamplingPeriod(symbolPeriod);
	outputSignals[1]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
	outputSignals[1]->setSymbolPeriod(symbolPeriod);
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
	/*
	if((inSignalType1 != BinaryValue) || (inSignalType2 != BinaryValue) || ){ 
		cout << "ERRO: demux_1_2.cpp - invalide signal type" << "\n"; 
		return false;
	}
	*/
	if ((inSignalType1 == BinaryValue) && (inSignalType2 == BinaryValue)) {
		t_binary binValue;
		t_binary logicalValue;

		for (int i = 0; i < length; i++) {
			inputSignals[0]->bufferGet(&binValue);
			inputSignals[1]->bufferGet(&logicalValue);

			if (logicalValue == 1) {
				outputSignals[0]->bufferPut((t_binary)binValue);
			}
			else if (logicalValue == 0)
				outputSignals[1]->bufferPut((t_binary)binValue);
			else
				cout << "Invalyde logical value, demux_1_2.cpp \n";
		}
	}
	else if ((inSignalType1 == RealValue) && (inSignalType2 == BinaryValue)) {
		t_real binValue;
		t_binary logicalValue;

		for (int i = 0; i < length; i++) {
			inputSignals[0]->bufferGet(&binValue);
			inputSignals[1]->bufferGet(&logicalValue);

			if (logicalValue == 1) {
				outputSignals[0]->bufferPut((t_binary)binValue);
			}
			else if (logicalValue == 0)
				outputSignals[1]->bufferPut((t_binary)binValue);
			else
				cout << "Invalyde logical value, demux_1_2.cpp \n";
		}
	}
	else {
		cout << "ERRO: demux_1_2.cpp: invalid type! \n";
	}

	return true;
}

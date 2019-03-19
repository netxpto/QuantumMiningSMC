#include "binary_to_ascii_20180905.h"

void BinaryToAscii::initialize(void)
{
	numberOfOutputSignals = (int)outputSignals.size();
	numberOfInputSignals = (int)inputSignals.size();

	if (outputSignals[0]->getType() != "Ascii") throw exception("All output signals must be of type Ascii.");
	if (inputSignals[0]->getType() != "Binary") throw exception("All input signals must be of type Binary.");
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	outputSignals[0]->setCentralWavelength(inputSignals[0]->getCentralWavelength());
	outputSignals[0]->setCentralFrequency(inputSignals[0]->getCentralFrequency());
}

bool BinaryToAscii::runBlock(void)
{
	long int process = std::min(inputSignals[0]->ready(),outputSignals[0]->space()*8);
	if (process == 0) {
		if (bitIndex != 0) {
			outputSignals[0]->bufferPut(aux);
			bitIndex = 0;
		}
		return false;
	}
	//For every 8 bits fetched from the binary signal, 1 character is put in the ascii signal
	for (int i = 0; i < process; i++) {
		if (bitIndex == 7) {
			t_binary bit;
			inputSignals[0]->bufferGet(&bit);
			aux = aux | (bit << (7 - bitIndex));
			outputSignals[0]->bufferPut(aux);
			bitIndex = -1;
			aux = '\0';
			//i--;
		} else {
			t_binary bit;
			inputSignals[0]->bufferGet(&bit);
			aux = aux | (bit << (7-bitIndex));
		}
		bitIndex += 1;
	}
	return true;
}

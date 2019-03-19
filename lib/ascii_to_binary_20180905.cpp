#include "ascii_to_binary_20180905.h"

void AsciiToBinary::initialize(void)
{
	numberOfOutputSignals = (int)outputSignals.size();
	numberOfInputSignals  = (int)inputSignals.size() ;

	if (outputSignals[0]->getType() != "Binary") throw exception("All output signals must be of type Binary.");
	if (inputSignals[0]->getType() != "Ascii") throw exception("All input signals must be of type Ascii.");
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	outputSignals[0]->setCentralWavelength(inputSignals[0]->getCentralWavelength());
	outputSignals[0]->setCentralFrequency(inputSignals[0]->getCentralFrequency());

}

bool AsciiToBinary::runBlock(void)
{
	int pendingChar = bitIndex == 0 ? 0 : 1;
	long int process = std::min((inputSignals[0]->ready()+pendingChar)*8-bitIndex,outputSignals[0]->space());
	if (process == 0) return false;

	for (long int k = 0; k < process; k++) {
		if (bitIndex == 0) inputSignals[0]->bufferGet(&aux);
		bitset<8> set = bitset<8>(aux);
		outputSignals[0]->bufferPut((t_binary)set[7-bitIndex]);
		bitIndex++;
		bitIndex = bitIndex % 8;
	}
	return true;
}

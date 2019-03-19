#include "ascii_source_20180828.h"

void AsciiSource::initialize(void)
{
	numberOfOutputSignals = (int)outputSignals.size();

	for (auto i = 0; i < numberOfOutputSignals; ++i) {
		outputSignals[i]->setSamplingPeriod(outputSignals[i]->getSymbolPeriod());
		outputSignals[i]->setSamplesPerSymbol(1.0);
		outputSignals[i]->setFirstValueToBeSaved(1);
	}

	input_file.open(asciiFilePath, ios::binary);
	//If file exists the program will load its content to asciiString. Else asciiString will assume its default value: "ABCabc"
	if (input_file) asciiString = std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
	else throw exception("ERROR: Cannot open file.");
}

bool AsciiSource::runBlock(void)
{

	long int space = outputSignals[0]->space();
	for (auto k = 1; k < numberOfOutputSignals; k++) {
		long int aux = outputSignals[k]->space();
		space = std::min(space, aux);
	}
	long int process = std::min(space, (long)numberOfCharacters);
	if (mode == Terminate) process = std::min((long)asciiString.length()-charIndex,process);
	if (process <= 0) return false;

	for (int i = 0; i < process; i++) {
		if (mode == AppendZeros && charIndex == asciiString.length()) {
			for (int k = 0; k < numberOfOutputSignals; k++) {
				outputSignals[k]->bufferPut('\0');
			}
		} else {
			for (int k = 0; k < numberOfOutputSignals; k++) {
				char aux = asciiString[charIndex];
				outputSignals[k]->bufferPut(aux);
			}
			charIndex += 1;
			if (mode == Cyclic) charIndex = charIndex % asciiString.length();
		}
		numberOfCharacters--;
	}

	return true;
}

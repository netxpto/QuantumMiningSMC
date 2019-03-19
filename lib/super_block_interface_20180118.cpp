
# include "super_block_interface_20180118.h"

using namespace std;


SuperBlockInterface::SuperBlockInterface(vector<Signal *> &inputSignals, vector<Signal *> &outputSignals) :Block(inputSignals, outputSignals) {
	
	setNumberOfInputSignals(inputSignals.size());
	setNumberOfOutputSignals(outputSignals.size());

	inputSignals = inputSignals;
	outputSignals = outputSignals;

}

void SuperBlockInterface::initialize(void) {

	for (auto k = 0; k < numberOfOutputSignals; k++) {
		outputSignals[k]-> setSymbolPeriod(inputSignals[k]->getSymbolPeriod());
		outputSignals[k]-> setSamplingPeriod(inputSignals[k]->getSamplingPeriod());
		outputSignals[k]-> setSamplesPerSymbol(inputSignals[k]->getSamplesPerSymbol());
		outputSignals[k]-> setFirstValueToBeSaved(inputSignals[k]->getFirstValueToBeSaved());
	}
}

bool SuperBlockInterface::runBlock(void) {

	bool alive{ false };

	std::size_t numberOfSignals = getNumberOfInputSignals();
	if (numberOfSignals != numberOfOutputSignals) {
		cout << "ERRO: super_block_interface.cpp (1)" << "\n";
	}

	for (int k = 0; k < numberOfSignals; k++) {
		int ready = inputSignals[k]->ready();
		int space = outputSignals[k]->space();

		int length = min(ready, space);
		if (length > 0) { alive = true; }

		signal_value_type inSignalType = inputSignals[k]->getValueType();
		switch (inSignalType) {
			case BinaryValue:
				for (int l = 0; l < length; l++) {
					t_binary value;
					inputSignals[k]->bufferGet(&value);
					outputSignals[k]->bufferPut((t_binary) value);
				}
				break;
			case RealValue:
				for (int l = 0; l < length; l++) {
					t_real value;
					inputSignals[k]->bufferGet(&value);
					outputSignals[k]->bufferPut((t_real) value);
				}
				break;
			case ComplexValue:
				for (int l = 0; l < length; l++) {
					t_complex value;
					inputSignals[k]->bufferGet(&value);
					outputSignals[k]->bufferPut((t_complex) value);
				}
				break;
			case ComplexValueXY:
				for (int l = 0; l < length; l++) {
					t_complex_xy value;
					inputSignals[k]->bufferGet(&value);
					outputSignals[k]->bufferPut((t_complex_xy) value);
				}
				break;
			case Message:
				for (int l = 0; l < length; l++) {
					t_message value;
					inputSignals[k]->bufferGet(&value);
					outputSignals[k]->bufferPut((t_message)value);
				}
				break;
		default:
			cout << "ERRO: super_block_interface.cpp (2)" << "\n";
			return false;
		};


	}
	
	return alive;

};

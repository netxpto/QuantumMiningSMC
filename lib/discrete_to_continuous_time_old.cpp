# include <algorithm> // min

# include "netxpto.h"
# include "discrete_to_continuous_time.h"

void DiscreteToContinuousTime::initialize(void) {

	outputSignals[0]->symbolPeriod = (inputSignals[0]->symbolPeriod);
	outputSignals[0]->samplingPeriod = (inputSignals[0]->samplingPeriod) / numberOfSamplesPerSymbol;
	outputSignals[0]->samplesPerSymbol = numberOfSamplesPerSymbol;
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}

bool DiscreteToContinuousTime::runBlock(void) {

	bool alive{ false };

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();
	

	if (index != 0) {
		for (int i = index; (i < numberOfSamplesPerSymbol) & (space>0); i++) {
			outputSignals[0]->bufferPut(0);
			alive = true;
			space--;
			index++;
		};
		if (index == numberOfSamplesPerSymbol) index = 0;
	};

	int length = min((int)ceil((double)space / (double)numberOfSamplesPerSymbol), ready);

	if (length <= 0) return alive;

	signal_value_type inSignalType = inputSignals[0]->getValueType();
	switch (inSignalType) {
		case RealValue:
			for (int i = 0; i < length; i++) {
				t_real value;
				(inputSignals[0])->bufferGet(&value);
				outputSignals[0]->bufferPut(value);
				contador++;
				space--;
				index++;
				for (int k = 1; (k<numberOfSamplesPerSymbol) & (space>0); k++) {
					outputSignals[0]->bufferPut((t_real) 0.0);
					space--;
					index++;
				}
				if (index == numberOfSamplesPerSymbol) index = 0;
			}
			return true;
		case BinaryValue:
			for (int i = 0; i < length; i++) {
				t_binary value;
				(inputSignals[0])->bufferGet(&value);
				if (value == 0) {
					outputSignals[0]->bufferPut((t_real) 0.0);
				}
				else {
					outputSignals[0]->bufferPut((t_real) 1.0);
				}
				space--;
				index++;
				for (int k = 1; (k<numberOfSamplesPerSymbol) & (space>0); k++) {
					outputSignals[0]->bufferPut((t_real) 0.0);
					space--;
					index++;
				}
				if (index == numberOfSamplesPerSymbol) index = 0;
			}
			return true;
		default:
			cout << "ERRO: discrete_to_continuous_time.cpp" << "\n";
			return false;
	};

};

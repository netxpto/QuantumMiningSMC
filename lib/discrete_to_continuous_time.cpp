# include <algorithm> // min

# include "netxpto_20180118.h"
# include "discrete_to_continuous_time.h"

void DiscreteToContinuousTime::initialize(void) {

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod((inputSignals[0]->getSamplingPeriod()) / numberOfSamplesPerSymbol);
	outputSignals[0]->setSamplesPerSymbol(numberOfSamplesPerSymbol);
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}

bool DiscreteToContinuousTime::runBlock(void) {

	bool alive{ false };

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();
	
	signal_value_type inSignalType = inputSignals[0]->getValueType();

	if (index != 0) {
		int length = min(numberOfSamplesPerSymbol - index, space);
		if (length > 0) alive = true;

		switch (inSignalType) {
			case RealValue:
				for (int i = 0; i < length; i++) {
					outputSignals[0]->bufferPut((t_real) 0.0);
					index++;
				}
				index = index % numberOfSamplesPerSymbol;
				break;
			default: cout << "ERRO: discrete_to_continuous_time.cpp - invalide signal type" << "\n";
		}
		
	};

	if (index != 0) return alive;

	int length = min(space, ready*numberOfSamplesPerSymbol);
	if (length <= 0) return alive;

	switch (inSignalType) {
		case RealValue:
			t_real value;
			for (int i = 0; i < length; i++) {
				if (index != 0) {
					outputSignals[0]->bufferPut((t_real) 0.0);
				}
				else {
					(inputSignals[0])->bufferGet(&value);
					outputSignals[0]->bufferPut(value);
				}
				space--;
				index++;
				index = index % numberOfSamplesPerSymbol;
			}
			break;
		default: cout << "ERRO: discrete_to_continuous_time.cpp - invalide signal type" << "\n";
	};
	return alive;
};

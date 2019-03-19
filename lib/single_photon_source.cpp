# include <algorithm>  // min

# include "netxpto.h"
# include "single_photon_source.h"

void SinglePhotonSource::initialize(void) {

	inputSignalValueType = inputSignals[0]->getValueType();

	outputSignals[0]->symbolPeriod = inputSignals[0]->symbolPeriod;
	outputSignals[0]->samplingPeriod = inputSignals[0]->samplingPeriod;
	outputSignals[0]->samplesPerSymbol = inputSignals[0]->samplesPerSymbol;
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}

bool SinglePhotonSource::runBlock(void) {

	int ready, ready0, ready1, space, length;

	switch (inputSignalValueType) {
	
		case RealValue:

			ready = inputSignals[0]->ready();
			space = outputSignals[0]->space();

			length = min(ready, space);

			if (length <= 0) return false;

			for (int i = 0; i < length; i++) {
				t_real value;
				t_photon photon;
				inputSignals[0]->bufferGet(&value);
				if (value == 1) {
					photon.probabilityAmplitude = 1;
					photon.polarization = polarization[0];
					outputSignals[0]->bufferPut(photon);
				}
				else {
					photon.probabilityAmplitude = 0;
					photon.polarization = 0;
					outputSignals[0]->bufferPut(photon);
				}
			}
			break;

		case BinaryValue:

			ready0 = inputSignals[0]->ready();
			ready1 = inputSignals[1]->ready();
			ready = min(ready0, ready1);

			space = outputSignals[0]->space();

			length = min(ready, space);

			if (length <= 0) return false;

			for (int i = 0; i < length; i++) {

				t_photon photon;
				t_binary value0;
				t_binary value1;

				inputSignals[0]->bufferGet(&value0);
				inputSignals[1]->bufferGet(&value1);

				t_integer value = 2 * value0 + value1;

				photon.probabilityAmplitude = 1;
				photon.polarization = polarization[value];
				outputSignals[0]->bufferPut(photon);
			}
			break;
	}

	return true;

};

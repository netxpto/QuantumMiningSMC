# include <algorithm>  // min()
# include <random>
# include <math.h>	   // remainder(), pow()

# include "netxpto.h"
# include "single_photon_detector.h"

void SinglePhotonDetector::initialize(void) {

	outputSignals[0]->symbolPeriod = inputSignals[0]->symbolPeriod;
	outputSignals[0]->samplingPeriod = inputSignals[0]->samplingPeriod;
	outputSignals[0]->samplesPerSymbol = inputSignals[0]->samplesPerSymbol;
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}

bool SinglePhotonDetector::runBlock(void) {

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int length = min(ready, space);

	if (length <= 0) return false;


	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(0.0, 1.0);

	signal_value_type inSignalType = inputSignals[0]->getValueType();
	switch (inSignalType) {
		case PhotonValue:
			for (int k = 0; k < length; k++) {
				t_photon inValue;
				inputSignals[0]->bufferGet(&inValue);
				if (inValue.probabilityAmplitude == 1) {
					outputSignals[0]->bufferPut((t_real) 1.0);
				}
				else {
					outputSignals[0]->bufferPut((t_real) 0.0);
				}
			}
			return true;
		case PhotonValueMP:
			for (int k = 0; k < length; k++) {
				t_photon_mp inValueMP;
				inputSignals[0]->bufferGet(&inValueMP);
				if (inValueMP.path[path].probabilityAmplitude > 0.0) {	// Process Photon Path
					double number = distribution(generator);
					if (number < pow(inValueMP.path[path].probabilityAmplitude, 2)) {
						outputSignals[0]->bufferPut((t_real) 1.0);
						inValueMP.path[(path + 1) % 2].probabilityAmplitude = 0.0;
						inValueMP.path[path].probabilityAmplitude = -1.0; // Photon Path Processed
					}
					else {
						outputSignals[0]->bufferPut((t_real) 0.0);
						inValueMP.path[(path + 1) % 2].probabilityAmplitude = 1.0;
						inValueMP.path[path].probabilityAmplitude = -1.0; // Photon Path Processed
					}
				};
				if (inValueMP.path[path].probabilityAmplitude == 0.0) {
					outputSignals[0]->bufferPut((t_real) 0.0);
					inValueMP.path[path].probabilityAmplitude = -1.0; // Photon Path Processed
				}
				if (inValueMP.path[(path + 1) % 2].probabilityAmplitude >= 0.0) {
						inputSignals[0]->bufferPut((t_photon_mp)inValueMP);
				}
			}
			return true;
		default:
			cout << "ERRO: single_photon_detector.cpp" << "\n";
			return false;
	};
};

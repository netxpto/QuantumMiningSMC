# include "single_photon_detector_20180111.h"

void SinglePhotonDetector::initialize(void) {

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}

bool SinglePhotonDetector::runBlock(void) {

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process <= 0) return false;


	std::uniform_real_distribution<double> distribution(0.0, 1.0);

	unsigned int seed = (unsigned int)chrono::system_clock::now().time_since_epoch().count();

	generator.seed(seed);

	signal_value_type inSignalType = inputSignals[0]->getValueType();
	switch (inSignalType) {
	case PhotonValue:
		for (auto k = 0; k < process; k++) {
			t_photon inValue;
			inputSignals[0]->bufferGet(&inValue);
			if (inValue.probabilityAmplitude == 1) {
				outputSignals[0]->bufferPut((t_real) 1.0);
			}
			else {
				outputSignals[0]->bufferPut((t_real) 0.0);
			}
		}
		break;
	case PhotonValueMP:
		for (auto k = 0; k < process; k++) {
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
		break;

	case PhotonValueMPXY:
		for (auto k = 0; k < process; k++) {
			t_photon_mp_xy inValueMP;
			inputSignals[0]->bufferGet(&inValueMP);
			t_complex_xy inValue = (t_complex_xy)inValueMP.path[path];
			t_complex xValue = inValue.x;
			t_complex yValue = inValue.y;



			switch (path) {
			case 0:
				if ((abs(xValue) >= 0.0) && (abs(xValue) <= 1.0)) {
					double number = distribution(generator);
					if (number < pow(abs(xValue), 2)) {
						outputSignals[0]->bufferPut((t_real)1.0);
						inValueMP.path[1].x = (t_complex) 0.0;
						inValueMP.path[1].y = (t_complex) 0.0;
					}
					else {
						outputSignals[0]->bufferPut((t_real)0.0);
						inValueMP.path[1].x = (t_complex) 0.0;
						inValueMP.path[1].y = (t_complex) 1.0;
					}
				}

				break;

			case 1:
				if (abs(yValue) == 1.0)
					outputSignals[0]->bufferPut((t_real)1.0);
				else
					outputSignals[0]->bufferPut((t_real)0.0);
				break;

			default:
				cout << "x:" << abs(xValue) << "y:" << abs(yValue) << "\n";
				break;

			}
	
			if ((abs(inValueMP.path[1].y) == 0.0) || (abs(inValueMP.path[1].y) == 1.0) ) {
				inputSignals[0]->bufferPut((t_photon_mp_xy)inValueMP);
			}
		}

		break;
	default:
		cout << "ERRO: single_photon_detector.cpp" << "\n";
		return false;
	};

	return true;
};

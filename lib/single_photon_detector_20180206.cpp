# include "single_photon_detector_20180206.h"

void SinglePhotonDetector::initialize(void) {

	numberOfInputSignals = (int)inputSignals.size();
	numberOfOutputSignals = (int)outputSignals.size();

	outputSignals[0]->setSymbolPeriod(inputSignals[1]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[1]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[1]->getSamplesPerSymbol());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[1]->getFirstValueToBeSaved());
	
}

bool SinglePhotonDetector::runBlock(void) {

	if (numberOfInputSignals > 2) {
		cout << "ERRO: singlephotondetector.cpp (1)" << "\n";
		return false;
	}
	else {
		int ready;
		if (numberOfInputSignals == 1) {
			ready = inputSignals[0]->ready();
		}
		else if (numberOfInputSignals == 2) {
			int ready1 = inputSignals[0]->ready();
			int ready2 = inputSignals[1]->ready();
			ready = min(ready1, ready2);
		}

		int space = outputSignals[0]->space();
		int process = min(ready, space);

		if (process <= 0) return false;

		std::uniform_real_distribution<double> distribution(0.0, 1.0);

		unsigned int seed = (unsigned int)chrono::system_clock::now().time_since_epoch().count();

		generator.seed(seed);

		signal_value_type inSignalType = inputSignals[0]->getValueType();

		switch (inSignalType) {

		case PhotonValueMPXY:

			for (auto k = 0; k < process; k++) {
			t_real clk;
			if (numberOfInputSignals > 1) {
				inputSignals[1]->bufferGet(&clk);
				
			}
			else 
				clk = 1.0;

			t_photon_mp_xy inValueMP;
			inputSignals[0]->bufferGet(&inValueMP);

			t_complex_xy inValue = (t_complex_xy)inValueMP.path[path];
			t_complex xValue = inValue.x;
			t_complex yValue = inValue.y;
			bool write{ false };

			switch (path) {
			case 0:
				if ((abs(xValue) >= 0.0) && (abs(xValue) <= 1.0)) {
					double number = distribution(generator);
					if ((number < (pow(abs(xValue), 2) + probabilityDarkCount))) {
						outputSignals[0]->bufferPut((t_real)1.0*clk);
						inValueMP.path[1].x = (t_complex) 0.0;
						inValueMP.path[1].y = (t_complex) 0.0;
						inValueMP.path[0].x = (t_complex) 1.0;
						inValueMP.path[0].y = (t_complex) 0.0;
						write = true;
					}
					else {
						outputSignals[0]->bufferPut((t_real)0.0*clk);
						inValueMP.path[1].x = (t_complex) 0.0;
						inValueMP.path[1].y = (t_complex) 1.0;
						inValueMP.path[0].x = (t_complex) 0.0;
						inValueMP.path[0].y = (t_complex) 0.0;
						
						write = true;
					}
				}
				else {
					write = true;
					outputSignals[0]->bufferPut((t_real)0.0*clk);
				}
					
				break;

			case 1:
				
				if ((abs(inValueMP.path[1].y) == 1.0)) {
					outputSignals[0]->bufferPut((t_real)1.0*clk);
				}	
				else {
					double number = distribution(generator);
					if ((number < probabilityDarkCount)) {
						outputSignals[0]->bufferPut((t_real)1.0*clk);
					}
					else {
						outputSignals[0]->bufferPut((t_real)0.0*clk);
					}
				}
				
				break;

			default:
				cout << "x:" << abs(xValue) << "y:" << abs(yValue) << "\n";
				break;

			}

			if(write)
				inputSignals[0]->bufferPut((t_photon_mp_xy)inValueMP);
			
		}

			break;
		default:
			cout << "ERRO: single_photon_detector.cpp" << "\n";
			return false;
		};

		

	}

	return true;
};

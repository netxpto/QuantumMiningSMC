# include "../include/single_photon_detector_20181113.h"

void SinglePhotonDetector::initialize(void) {

	numberOfInputSignals = (int)inputSignals.size();
	numberOfOutputSignals = (int)outputSignals.size();

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	
}

bool SinglePhotonDetector::runBlock(void)
{

	if (numberOfInputSignals > 2)
	{
		cout << "ERRO: singlephotondetector.cpp - number of input signals" << "\n";
		return false;
	}
	else
	{

		auto ready = (numberOfInputSignals == 1) ? inputSignals[0]->ready() : min(inputSignals[0]->ready(), inputSignals[1]->ready());

		auto space = outputSignals[0]->space();
		auto process = min(ready, space);

		if (process <= 0) return false;

		signal_value_type inSignalType = inputSignals[0]->getValueType();
		signal_value_type outSignalType = outputSignals[0]->getValueType();

		switch (inSignalType)
		{
		case signal_value_type::t_photon_mp_xy:
			for (auto k = 0; k < process; k++) {
				double on{ 1.0 };
				if (numberOfInputSignals > 1)
				{
					t_real window;
					inputSignals[1]->bufferGet(&window);
					if (window == 0) on = 0;
				}

				t_photon_mp_xy inValueMP;
				inputSignals[0]->bufferGet(&inValueMP);

				t_real out{ 0.0 };

				t_integer otherPath = (path == 0) ? 1 : 0;

				double probabilityOfClick = round(pow(abs(inValueMP.path[path].x), 2) + pow(abs(inValueMP.path[path].y), 2) + pow(abs(inValueMP.path[otherPath].x), 2) + pow(abs(inValueMP.path[otherPath].y), 2));

				double probabilityOfThisDetectorClick = pow(abs(inValueMP.path[path].x), 2) + pow(abs(inValueMP.path[path].y), 2);

				if (probabilityOfClick > 1) cout << "ERRO: single_photon_detector.cpp - probability of click greater than 1" << "\n";
				
				double number = uniformeRandomNumberBetweenZeroAndOne();

				if (number < probabilityOfThisDetectorClick)
				{
					out = 1.0;
					inValueMP.path[otherPath].x = (t_complex) 0.0;
					inValueMP.path[otherPath].y = (t_complex) 0.0;
				}
				else
				{
					out = 0.0;
					double probabilityOfTheOtherDetectorClick = pow(abs(inValueMP.path[otherPath].x), 2) + pow(abs(inValueMP.path[otherPath].y), 2);
					inValueMP.path[path].x = (t_complex) 0.0;
					inValueMP.path[path].y = (t_complex) 0.0;
					//inValueMP.path[otherPath].x = (probabilityOfTheOtherDetectorClick == 0) ? 0 : sqrt(1 / probabilityOfTheOtherDetectorClick)*inValueMP.path[otherPath].x;
					//inValueMP.path[otherPath].y = (probabilityOfTheOtherDetectorClick == 0) ? 0 : sqrt(1 / probabilityOfTheOtherDetectorClick)*inValueMP.path[otherPath].y;
					inValueMP.path[otherPath].x = (probabilityOfTheOtherDetectorClick == 0) ? 0 : sqrt(1 / probabilityOfTheOtherDetectorClick)*inValueMP.path[otherPath].x;
					inValueMP.path[otherPath].y = (probabilityOfTheOtherDetectorClick == 0) ? 0 : sqrt(1 / probabilityOfTheOtherDetectorClick)*inValueMP.path[otherPath].y;
					probabilityOfClick = round(pow(abs(inValueMP.path[otherPath].x), 2) + pow(abs(inValueMP.path[otherPath].y), 2));
					if ((probabilityOfClick != 0) && (probabilityOfClick != 1)) 
						cout << "ERRO: single_photon_detector.cpp - wrong probability of click on the other detector" << "\n";
				}
				
				inValueMP.path[path].processed = true;

				outputSignals[0]->bufferPut((t_real)out*on);

				if (inValueMP.path[otherPath].processed == false) {
					inputSignals[0]->bufferPut((t_photon_mp_xy)inValueMP);
				}
			}
			break;
		default:
			cout << "ERRO: single_photon_detector.cpp - input signal type" << "\n";
			return false;
		};

		return true;
	};
};

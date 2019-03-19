# include "iq_modulator_20180130.h"


using namespace std;


void IqModulator::initialize(void) {

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	outputSignals[0]->setCentralWavelength(outputOpticalWavelength);
	outputSignals[0]->setCentralFrequency(outputOpticalFrequency);

}

bool IqModulator::runBlock(void) {
	
	int numberOfInpuSignals = inputSignals.size();
	double symbolPeriod = outputSignals[0]->getSymbolPeriod();
	if (numberOfInputSignals == 2) {

		int ready0 = inputSignals[0]->ready();
		int ready1 = inputSignals[1]->ready();
		int ready = min(ready0, ready1);

		int space = outputSignals[0]->space();

		int process = min(ready, space);

		if (process == 0) return false;

		t_real re, im;
		for (int i = 0; i < process; i++) {

			inputSignals[0]->bufferGet(&re);
			inputSignals[1]->bufferGet(&im);

			t_complex valueX(re, im);
			valueX = 0.5*sqrt(outputOpticalPower)*valueX;

			signal_value_type sType = outputSignals[0]->getValueType();

			switch (sType) {
			case ComplexValue:
				outputSignals[0]->bufferPut(valueX);
				break;
			case ComplexValueXY:
				t_complex valueY(0, 0);
				t_complex_xy valueXY = { valueX, valueY };
				outputSignals[0]->bufferPut(valueXY);
				break;
			}


		}

		return true;
	}

	if (numberOfInputSignals == 3) {

		
		int readyI = inputSignals[0]->ready();
		int readyQ = inputSignals[1]->ready();
		int readyOpticalSignal = inputSignals[2]->ready();

		int ready = min(readyOpticalSignal, readyI);
		ready = min(ready, readyQ);

		int space = outputSignals[0]->space();

		int process = min(ready, space);

		if (process == 0) return false;

		t_complex opticalIn;

		for (int i = 0; i < process; i++) {

			inputSignals[2]->bufferGet(&opticalIn);

			t_real re, im;
			inputSignals[0]->bufferGet(&re);
			inputSignals[1]->bufferGet(&im);

			t_complex valueX(re, im);
			valueX = opticalIn*valueX;

			signal_value_type sType = outputSignals[0]->getValueType();

			switch (sType) {
			case ComplexValue:
				outputSignals[0]->bufferPut(valueX);
				break;
			case ComplexValueXY:
				t_complex valueY(0, 0);
				t_complex_xy valueXY = { valueX, valueY };
				outputSignals[0]->bufferPut(valueXY);
				break;
			}


		}

		return true;
	}

	cout << "ERROR: number of IQ Modulator wrong number of input signals\n";
	return false;

}

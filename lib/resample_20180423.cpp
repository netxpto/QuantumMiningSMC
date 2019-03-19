# include "resample_20180423.h"

# include <algorithm>
# include <complex>
# include <iostream>
# include <fstream>
# include <random>

using namespace std;


void Resample::initialize(void) {

	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	//outputSignals[0]->setSymbolPeriod(symbolPeriod);
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	//outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
}


bool Resample::runBlock(void) {


	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process == 0) return false;
	
	double samplingPeriod = outputSignals[0]->getSamplingPeriod();
	double samplingRate = 1 / samplingPeriod;
	t_real var(0.0);

	if (rFactor == 1.0) {
		
		for (int i = 0; i < process; i++) {

			inputSignals[0]->bufferGet(&var);
			outputSignals[0]->bufferPut(var);
		}
		return true;
	}

	if (rFactor > 1.0) {

		int l = 1;
		double xi = 0.0;
		int processLoop = (int) floor(process*rFactor);
		vector<complex <double>> bufferInput(process);
		vector<complex <double>> bufferOutput(processLoop);
		for (int i = 0; i < process; i++) {

			inputSignals[0]->bufferGet(&var);
			bufferInput.at(i) = var;
		}

		for (int k = 0; k < processLoop; ++k) {

			if ((k != bufferOutput.size() - 1) && (k != 0)) {
				l = (int) ceil(xi);
			}
			bufferOutput.at(k) = bufferInput.at(l - 1) + (xi - (l - 1))*((bufferInput.at(l) - bufferInput.at(l - 1))/(double(l) - (l - 1)));

			xi = xi + 1.0 / double(rFactor);
		}

		for (int i = 0; i < rFactor*process; i++) {

			outputSignals[0]->bufferPut(bufferOutput.at(i));

		}
		return true;
	}
	
	cout << "ERROR: Invalid resample factor\n";
	return false;
}

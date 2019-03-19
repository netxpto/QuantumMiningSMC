# include "frequency_mismatch_compensation.h"

# include <algorithm>
# include <complex>
# include <iostream>
# include <fstream>
# include <random>

using namespace std;


void FrequencyMismatchCompensation::initialize(void) {

	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());

}


bool FrequencyMismatchCompensation::runBlock(void) {

	int process = outputSignals[0]->space();
	int signalSize = process;

	if (process == 0) return false;
	t_real in;
	double *aux;
	aux = (double *)malloc(process * sizeof(aux));
	t_real out;
	double frequencyEstimate;

	for (int i = 0; i < process; i++) {
		inputSignals[0]->bufferGet(&in);
		aux[i] = in;
		
		//out = 0;

		//outputSignals[0]->bufferPut((t_complex)out);
	}

	return true;
}

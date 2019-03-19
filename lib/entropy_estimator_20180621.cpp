#include <algorithm>
#include <iostream>
#include <Windows.h>

#include "netxpto_20180418.h"
#include "entropy_estimator_20180621.h"

using namespace std;

void EntropyEstimator::initialize(void) {
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
}

bool EntropyEstimator::runBlock(void) {
	bool alive{ false };

	/* Avaiable bits on input buffer */
	int ready = inputSignals[0]->ready();

	/* Avaiable space on output buffer */
	int space = outputSignals[0]->space();

	/* Cycle to process data */
	while ((ready > 0) && (space > 0)) {
		int in;

		inputSignals[0]->bufferGet(&in);
		totalSymbols++;

		if (in == 0) {
			counter[0] += 1;
		}
		else {
			counter[1] += 1;
		}

		probability[0] = counter[0] / (double)totalSymbols;
		probability[1] = counter[1] / (double)totalSymbols;


		double entropy = 0.0;
		if (probability[0] != 0){
			entropy += probability[0] * log2(1 / probability[0]);
		}
		
		if (probability[1] != 0) {
			entropy += probability[1] * log2(1 / probability[1]);
		}

		outputSignals[0]->bufferPut(entropy);

		ready = inputSignals[0]->ready();
		space = outputSignals[0]->space();
	}

	return alive;
}



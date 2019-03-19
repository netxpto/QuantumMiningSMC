# include "quantizer_20180423.h"

# include <algorithm>
# include <complex>
# include <iostream>
# include <fstream>
# include <random>

using namespace std;


void Quantizer::initialize(void) {

	outputSignals[0]->setSamplingPeriod(samplingPeriod);
	//outputSignals[0]->setSymbolPeriod(symbolPeriod);
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	//outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
}


bool Quantizer::runBlock(void) {


	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process == 0) return false;
	
	double samplingPeriod = outputSignals[0]->getSamplingPeriod();
	double samplingRate = 1 / samplingPeriod;

	double inf = std::numeric_limits<double>::infinity();
	t_real re;
	t_real var(0.0);
	if (nBits != inf) {

		t_real QLevel = pow(2, nBits - 1);

		/*for (int i = 0; i < process; i++) {

			// Normalize Signal
			inputSignals[0]->bufferGet(&var);
			re = var.real();
			im = var.imag();

			if (maxValue < abs(var.real())) maxValue = abs(var.real());
			if (maxValue < abs(var.imag())) maxValue = abs(var.imag());

		}*/

		for (int i = 0; i < process; i++) {

			// Quantize Signal
			inputSignals[0]->bufferGet(&var);
			
			re = var / maxValue;
			re = floor(re * QLevel + 0.5) / QLevel * maxValue;
			/*im = var.imag() / maxValue;
			im = floor(im * QLevel + 0.5) / QLevel * maxValue;*/

			t_real valueX = re;
			outputSignals[0]->bufferPut(valueX);
		}
		return true;
	}
	if (nBits == inf) {

		for (int i = 0; i < process; i++) {

			inputSignals[0]->bufferGet(&var);
			
			outputSignals[0]->bufferPut(var);
		}
		return true;
	}

	cout << "ERROR: Invalid quantization bit precision\n";
	return false;
}

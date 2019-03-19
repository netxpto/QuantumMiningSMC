#include "electrical_signal_generator_20180124.h"


void ElectricalSignalGenerator::initialize(void) {

	outputSignals[0]->setSamplingPeriod(samplingPeriod);
	int numberOfSamplesPerSymbol = (int) round(symbolPeriod / samplingPeriod);
	outputSignals[0]->setSamplesPerSymbol(numberOfSamplesPerSymbol);
	outputSignals[0]->setSymbolPeriod(symbolPeriod);
	outputSignals[0]->setFirstValueToBeSaved(1);

}

bool ElectricalSignalGenerator::runBlock(void) {
	long int space = outputSignals[0]->space();
	long int process = space;

	if (process <= 0) return false;
	
	if (signalFunction == ContinuousWave) {

		for (int i = 0; i <= process; i++) {
			outputSignals[0]->bufferPut((t_real)(1.0 * gainValue));
		}
	}

	return true;
}
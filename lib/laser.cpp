# include "netxpto.h"
# include "laser.h"

# include <algorithm>
# include <complex>
# include <iostream>
# include <fstream>
# include <random>

using namespace std;


void Laser::initialize(void) {

	outputSignals[0]->setSamplingPeriod(samplingPeriod);
	outputSignals[0]->setSymbolPeriod(symbolPeriod);

	outputSignals[0]->setCentralWavelength(getWavelength());
	outputSignals[0]->setCentralFrequency(getFrequency());
}


bool Laser::runBlock(void) {

	int process = outputSignals[0]->space();

	if (process == 0) return false;

	t_complex out(cos(phase), sin(phase));
	double outOpticalPower = opticalPower;
	double wavelength = outputSignals[0]->getCentralWavelength();
	double dt = outputSignals[0]->getSamplingPeriod();


	normal_distribution<double> distribution(0, 1);

	out = .5*sqrt(opticalPower)*out;

	for (int i = 0; i < process; i++) {
		if (signaltoNoiseRatio != 0)
		{
			t_real noise = distribution(generatorRIN);
			outOpticalPower = opticalPower + opticalPower / signaltoNoiseRatio*noise;
			out = .5*sqrt(outOpticalPower)*out;
		}
		outputSignals[0]->bufferPut((t_complex)out);
	}

	return true;
}
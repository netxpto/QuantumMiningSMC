# include "netxpto.h"
# include "lo_ideal.h"

# include <algorithm>
# include <complex>
# include <iostream>
# include <fstream>
# include <random>

using namespace std;


void LocalOscillator::initialize(void) {

	outputSignals[0]->setSamplingPeriod(samplingPeriod);
	outputSignals[0]->setSymbolPeriod(symbolPeriod);
	outputSignals[0]->setCentralWavelength(getWavelength());
	outputSignals[0]->setCentralFrequency(getFrequency());
}


bool LocalOscillator::runBlock(void) {

	/*int process = outputSignals[0]->space();

	if (process == 0) return false;

	//t_complex outAverage(cos(phase), sin(phase));
	t_complex out(cos(phase), sin(phase));
	double outOpticalPower = opticalPower;
	double wavelength = outputSignals[0]->getCentralWavelength();
	double deltaPhase = sqrt(PLANCK_CONSTANT*SPEED_OF_LIGHT / (2 * opticalPower*samplingPeriod*wavelength));
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
	}*/

	int process = outputSignals[0]->space();

	if (process == 0) return false;

	double dt = outputSignals[0]->getSamplingPeriod();

	for (int i = 0; i < process; i++)
	{
		phase = phase + 2 * PI*outputOpticalFrequency * dt;
		t_real X = .5*sqrt(opticalPower)*cos(phase);
		t_real Y = .5*sqrt(opticalPower)*sin(phase);
		t_complex out(X,Y);
		outputSignals[0]->bufferPut((t_complex)out);
	}

	return true;
}
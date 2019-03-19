# include "../include/local_oscillator_20190131.h"

/*
# include <algorithm>
# include <complex>
# include <iostream>
# include <fstream>
# include <random>
*/
# include <chrono>

using namespace std;


void LocalOscillator::initialize(void) {

	outputSignals[0]->setSamplingPeriod(samplingPeriod);
	outputSignals[0]->setSymbolPeriod(symbolPeriod);

	outputSignals[0]->setCentralWavelength(getWavelength());
	outputSignals[0]->setCentralFrequency(getFrequency());
}


bool LocalOscillator::runBlock(void) {

	int process = outputSignals[0]->space();
	int signalSize = process;

	if (process == 0) return false;

	t_real phNoiseVar, phNoise, intNoise, intVar;
	double cm_phNoise (0.0);
	t_complex out(0.0, 0.0);
	double samplingPeriod = outputSignals[0]->getSamplingPeriod();
	const double PI = 3.141592653589793;
	const complex<double> I(0, 1);

	normal_distribution<double> dist_phaseNoise;
	normal_distribution<double> dist_RIN;
	
	phNoiseVar = 2 * PI * laserLW * samplingPeriod;
	intVar = (1 / samplingPeriod) * laserRIN * pow(opticalPower, 2);

	unsigned seed = (unsigned)chrono::system_clock::now().time_since_epoch().count();

	generatorPhaseNoise.seed(seed);


	for (int i = 0; i < process; i++) {

		// Laser phase noise
		phNoise = sqrt(phNoiseVar)*dist_phaseNoise(generatorPhaseNoise); 
		cm_phNoise += phNoise;

		// Laser intensity noise
	    intNoise = sqrt(intVar)*dist_RIN(generatorRIN);

		// Laser transmitted optical field
		out = sqrt(opticalPower + intNoise)*exp(I*(cm_phNoise + phase0+frequencyMismatch*2*PI*time));

		// Time update

		time += samplingPeriod;

		outputSignals[0]->bufferPut((t_complex)out);
	}

	return true;
}

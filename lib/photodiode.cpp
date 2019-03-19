#include <algorithm>
#include <complex>
#include <chrono>
#include <fstream>

#include "netxpto.h"
#include "photodiode.h"
#include <random>


void Photodiode::initialize(void){

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}


bool Photodiode::runBlock(void){

	double samplingPeriod = inputSignals[0]->getSamplingPeriod();
	double symbolPeriod = inputSignals[0]->getSymbolPeriod ();
	int samplesPerSymbol = (int)round(symbolPeriod / samplingPeriod);

	int ready = inputSignals[0]->ready();

	int space = outputSignals[0]->space();

	int process = min(ready, space);
	
	if (process == 0) return false;

	if ((!getShotNoise()) & (getThermalNoiseAmplitude() == 0)) {

		t_complex input1;
		t_real power, current;
		for (int i = 0; i < process; i++) {

			t_complex input1;
			inputSignals[0]->bufferGet(&input1);

			// The 2 factor is compensating the bandpass signal representation
			// amplitude correction.
			power = 2 * abs(input1)*abs(input1);

			current = responsivity*power;

			outputSignals[0]->bufferPut(current);
		}
	};

	/*
	// Probability distribution
	normal_distribution<double> distribution(0, 1);
	double noiseAmpQuantum;
	double noiseAmpThermal;

	unsigned seed = (unsigned)chrono::system_clock::now().time_since_epoch().count();

	generatorAmp1.seed(seed);
	generatorAmp2.seed(seed);


	double wavelength = inputSignals[0]->getCentralWavelength();
	// Power constant for shotNoise.
	// P = power of 1 photon.
	t_real P = PLANCK_CONSTANT*SPEED_OF_LIGHT / (samplingPeriod*wavelength);

	t_real power, current;

	for (int i = 0; i < process; i++) {

		noiseAmpQuantum = distribution(generatorAmp1);
		noiseAmpThermal = distribution(generatorAmp2);

		t_complex input1;
		inputSignals[0]->bufferGet(&input1);


		// The 4 factor is compensating the bandpass signal representation
		// amplitude correction.
		power = abs(input1)*abs(input1) * 4;


		if (shotNoise)
		{
			// This approximation assumes that the number of photons is much
			// greater than 0.	
			power += sqrt(P)*noiseAmpQuantum * (sqrt(power) + sqrt(P)*noiseAmpQuantum / 4);
		}

		// Conversion of optical power to current.
		current = responsivity*power;
		
		
		// Implementation of thermal noise
		if (thermalNoise) {
			current += thermalNoiseAmplitude * noiseAmpThermal;
		}

		outputSignals[0]->bufferPut(current);
		
		// Extras auxiliares (ver definição da class)
		t = t + samplingPeriod;
		aux++;
		if (aux==samplesPerSymbol)
		{
			aux = 0;
		}

	}
	*/
	return true;
}

//  version: 20180302
//   author: Diamantino Silva
// whatsnew: outputs the number of detected photons

#include <algorithm>
#include <complex>
#include <chrono>
#include <fstream>

#include "netxpto_20180118.h"
#include "photoelectron_generator_20180302.h"
#include <random>


void PhotoelectronGenerator::initialize(void){

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}


bool PhotoelectronGenerator::runBlock(void){

	double samplingPeriod = inputSignals[0]->getSamplingPeriod();
	double symbolPeriod = inputSignals[0]->getSymbolPeriod ();
	int samplesPerSymbol = (int)round(symbolPeriod / samplingPeriod);
	
	int ready = inputSignals[0]->ready();
	
	int space = outputSignals[0]->space();
	
	int process = min(ready, space);
	 
	if (process == 0) return false;
	


	double wavelength = inputSignals[0]->getCentralWavelength();
	// Power of a single photon.
	t_real P = PLANCK_CONSTANT*SPEED_OF_LIGHT / (samplingPeriod*wavelength);
	// Input power
	t_real power;
	// Number of photons per interval
	t_real n_photons;


	//// No Noise ////
	if ((!getShotNoise()) & (getThermalNoiseAmplitude() == 0)) {
		
		t_complex input1;
		t_real power;
		for (int i = 0; i < process; i++) {
			
			t_complex input1;
			inputSignals[0]->bufferGet(&input1);
			
			// The 4 factor is compensating the bandpass signal representation
			// amplitude correction.
			power = 4 * abs(input1)*abs(input1);
			n_photons = power / P;
			
			outputSignals[0]->bufferPut(n_photons);
		}

		return true;
	};


	//// With Noise ////

	// Initial distribution with dummy parameter.
	// The Poisson distribution does not support 0.0 as mean.
	poisson_distribution<int> distribution(1);
	// Obtaining parameter type for latter use in cicle.
	using param_t = std::poisson_distribution<int>::param_type;
	
	unsigned seed = (unsigned)chrono::system_clock::now().time_since_epoch().count();
	generatorAmp.seed(seed);
	


	for (int i = 0; i < process; i++) {

		//noiseAmpQuantum = distribution(generatorAmp1);
		//noiseAmpThermal = distribution(generatorAmp2);

		t_complex input1;
		inputSignals[0]->bufferGet(&input1);


		// The 4 factor is compensating the bandpass signal representation
		// amplitude correction.
		power = abs(input1)*abs(input1) * 4;
		n_photons = power / P;

		// The efficiency is being applied before the poisson process because
		// the resulting random variable (binomial after poisson) is equal to a
		// poisson process with mean of first poisson times the efficiency.
		n_photons = n_photons * efficiency;

		// The poisson random number generator cannot take 0 as mean.
		if (shotNoise & (n_photons>0))
		{	
			// Random generator parameter configuration.
			distribution.param(param_t{ n_photons });
			n_photons = (t_real)distribution(generatorAmp);
		}

		
		// Implementation of thermal noise (future version)
		/*
		if (thermalNoise) {
			current += thermalNoiseAmplitude * noiseAmpThermal;
		}
		*/

		outputSignals[0]->bufferPut(n_photons);
		
		// Auxiliary extras (see class definition)
		t = t + samplingPeriod;
		aux++;
		if (aux==samplesPerSymbol)
		{
			aux = 0;
		}

	}
	
	return true;
}

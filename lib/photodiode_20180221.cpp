// version: 2018-02-21
//  author: Diamantino Silva

#include <algorithm>
#include <complex>
#include <chrono>
#include <fstream>

#include "netxpto_20180118.h"
#include "photodiode_20180221.h"
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
			
			// The 4 factor is compensating the bandpass signal representation
			// amplitude correction.
			power = 4 * abs(input1)*abs(input1);
			
			current = efficiency*power;
			
			outputSignals[0]->bufferPut(current);
		}
	};

	
	// Probability distribution
	// Initial distribution with default parameter.
	// Poisson distribution does not support 0.0 as mean.
	poisson_distribution<int> distribution(1);
	// Obtaining parameter type for latter use in cicle.
	using param_t = std::poisson_distribution<int>::param_type;
	
	unsigned seed = (unsigned)chrono::system_clock::now().time_since_epoch().count();
	generatorAmp.seed(seed);


	double wavelength = inputSignals[0]->getCentralWavelength();
	// Power constant for shotNoise.
	// P = power of a single photon.
	t_real P = PLANCK_CONSTANT*SPEED_OF_LIGHT / (samplingPeriod*wavelength);

	t_real power, current;
	// Number of photons
	t_real n_photons;

	for (int i = 0; i < process; i++) {

		//noiseAmpQuantum = distribution(generatorAmp1);
		//noiseAmpThermal = distribution(generatorAmp2);

		t_complex input1;
		inputSignals[0]->bufferGet(&input1);


		// The 4 factor is compensating the bandpass signal representation
		// amplitude correction.
		power = abs(input1)*abs(input1) * 4;
		n_photons = power / P;

		// The efficiency is applied before the poisson process (quantum noise) because the resulting random variable
		// (binomial after poisson) is equal to a poisson with mean of poisson times the efficiency.
		n_photons = n_photons * efficiency;


		// Configuração do gerador de números aleatórios
		distribution.param(param_t{ n_photons });

		if (shotNoise)
		{	
			// The poisson random number generator cannot take 0 as mean.
			if (power == 0.0) {
				n_photons = 0.0;
			}
			else {
				n_photons = (t_real)distribution(generatorAmp);
			}
		}

		// Conversion of optical power to current.
		power = n_photons * P;
		current = 1.0 * power;
		
		
		// Implementation of thermal noise
		/*
		if (thermalNoise) {
			current += thermalNoiseAmplitude * noiseAmpThermal;
		}
		*/

		outputSignals[0]->bufferPut(current);
		
		// Extras auxiliares (ver definição da class)
		t = t + samplingPeriod;
		aux++;
		if (aux==samplesPerSymbol)
		{
			aux = 0;
		}

	}
	
	return true;
}

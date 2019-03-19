# include "pulse_shaper_20180111.h"

using namespace std;

void raisedCosine(vector<t_real> &impulseResponse, int impulseResponseLength, double rollOffFactor, double samplingPeriod, double symbolPeriod, bool passiveFilterMode);
void gaussian(vector<t_real> &impulseResponse, int impulseResponseLength, double rollOffFactor, double samplingPeriod, double symbolPeriod, bool passiveFilterMode);
void square(vector<t_real> &impulseResponse, int impulseResponseLength, double samplingPeriod, double symbolPeriod, double pulseDelay, double pulseWidth);


void PulseShaper::initialize(void) {

	double samplingPeriod = inputSignals[0]->getSamplingPeriod();
	double symbolPeriod = inputSignals[0]->getSymbolPeriod();

	impulseResponseLength = (int)floor(impulseResponseTimeLength * symbolPeriod / samplingPeriod);
	impulseResponse.resize(impulseResponseLength);

	switch (getFilterType()) {

		case RaisedCosine:
			raisedCosine(impulseResponse, impulseResponseLength, rollOffFactor, samplingPeriod, symbolPeriod, passiveFilterMode);
			break;
		case Gaussian:
			gaussian(impulseResponse, impulseResponseLength, rollOffFactor, samplingPeriod, symbolPeriod, passiveFilterMode);
			break;
		case Square:
			square(impulseResponse, impulseResponseLength, samplingPeriod, symbolPeriod, pulseDelay, pulseWidth);
			break;
	};

	initializeFIR_Filter();
}

void raisedCosine(vector<t_real> &impulseResponse, int impulseResponseLength, double rollOffFactor, double samplingPeriod, double symbolPeriod, bool passiveFilterMode) {
	double sinc;
	double gain{ 0 };
	for (int i = 0; i < impulseResponseLength; i++) {
		t_real t = -impulseResponseLength / 2 * samplingPeriod + i * samplingPeriod;
		if (t != 0) {
			sinc = sin(PI * t / symbolPeriod) / (PI * t / symbolPeriod);
		}
		else {
			sinc = 1;
		}
		impulseResponse[i] = sinc*cos(rollOffFactor*PI*t / symbolPeriod) / (1 - (4.0 * rollOffFactor * rollOffFactor * t * t) / (symbolPeriod * symbolPeriod));
		gain = gain + impulseResponse[i];
	};
	if (passiveFilterMode)
	{
		for (int i = 0; i < impulseResponseLength; i++)
		{
			impulseResponse[i] = impulseResponse[i] / gain;
		}
	}
};

void gaussian(vector<t_real> &impulseResponse, int impulseResponseLength, double rollOffFactor, double samplingPeriod, double symbolPeriod, bool passiveFilterMode) {
	double gauss;
	double pulsewidth = 5e-10;
	double gain{ 0 };
	for (int i = 0; i < impulseResponseLength; i++) {
		t_real t = -impulseResponseLength / 2 * samplingPeriod + i * samplingPeriod;
		gauss = exp(-t*t / (pulsewidth*pulsewidth/36));
		impulseResponse[i] = gauss;
		gain = gain + impulseResponse[i];
	};
	if (passiveFilterMode)
	{
		for (int i = 0; i < impulseResponseLength; i++)
		{
			impulseResponse[i] = impulseResponse[i] / gain;
		}
	}
};

void square(vector<t_real> &impulseResponse, int impulseResponseLength, double samplingPeriod, double symbolPeriod, double pulseDelay, double pulseWidth) {

	int samplesPerSymbol = (int) (symbolPeriod / samplingPeriod);

	int pulseWidthLength = (int) floor(pulseWidth * samplesPerSymbol);
	int pulseDelaySamples = (int)floor(pulseDelay * samplesPerSymbol);

	pulseWidthLength = pulseWidthLength + pulseDelaySamples;

	for (int k = 0; k < impulseResponseLength; k++) {
		impulseResponse[k] = 0.0;
	};
	
	for (int k = pulseDelaySamples; k < pulseWidthLength; k++) {
		impulseResponse[k] = 1.0;
	};
/*
	for (int k = 0; k < samplesPerSymbol; k++) {
		impulseResponse[k] = 1.0;
	};

	for (int k = samplesPerSymbol; k < impulseResponseLength; k++) {
		impulseResponse[k] = 0.0;
	};
	*/
};

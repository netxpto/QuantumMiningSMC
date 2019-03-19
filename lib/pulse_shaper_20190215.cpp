# include "netxpto_20190215.h"
# include "pulse_shaper_20190215.h"

using namespace std;

void raisedCosine(vector<t_real> &impulseResponse, int impulseResponseLength, double rollOffFactor, double samplingPeriod, double symbolPeriod, bool passiveFilterMode);
void rootRaisedCosine(vector<t_real> &impulseResponse, int impulseResponseLength, double rollOffFactor, double samplingPeriod, double symbolPeriod, bool passiveFilterMode, bool normalizeEnergy);
void gaussian(vector<t_real> &impulseResponse, int impulseResponseLength, double rollOffFactor, double samplingPeriod, double symbolPeriod, bool passiveFilterMode);
void square(vector<t_real> &impulseResponse, int impulseResponseLength, double samplingPeriod, double symbolPeriod);


void PulseShaper::initialize(void) {

	double samplingPeriod= inputSignals[0]->getSamplingPeriod();
	double symbolPeriod= inputSignals[0]->getSymbolPeriod();

	impulseResponseLength = (int)floor(impulseResponseTimeLength * symbolPeriod / samplingPeriod);
	impulseResponse.resize(impulseResponseLength);

	switch (getFilterType()) {

		case RaisedCosine:
			raisedCosine(impulseResponse, impulseResponseLength, rollOffFactor, samplingPeriod, symbolPeriod, passiveFilterMode);
			break;
		case RootRaisedCosine:
			rootRaisedCosine(impulseResponse, impulseResponseLength, rollOffFactor, samplingPeriod, symbolPeriod, passiveFilterMode, rrcNormalizeEnergy);
			break;
		case Gaussian:
			gaussian(impulseResponse, impulseResponseLength, rollOffFactor, samplingPeriod, symbolPeriod, passiveFilterMode);
			break;
		case Square:
			square(impulseResponse, impulseResponseLength, samplingPeriod, symbolPeriod);
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

void rootRaisedCosine(vector<t_real> &impulseResponse, int impulseResponseLength, double rollOffFactor, double samplingPeriod, double symbolPeriod, bool passiveFilterMode, bool normalizeEnergy) {
	/*
	for idx = 1:length(tlist)
		t = tlist(idx);
		if t == 0
			ht(end + 1) = (1 / T)*(1 + b * ((4 / pi) - 1));
		elseif(abs(t) - (T / 4 * b)) == 0
			ht(end + 1) = (b / (T*sqrt(2)))*((1 + (2 / pi))*sin(pi / (4 * b)) + (1 - (2 / pi))*cos(pi / (4 * b)));
		else
			ht(end + 1) = (1 / T)*(sin((pi*t / T)*(1 - b)) + ((4 * b*t / T)*cos((pi*t / T)*(1 + b)))) / ((pi*t / T)*(1 - (4 * b*t / T) ^ 2));
		end
	end
	*/

	double gain{ 0 };
	for (int i = 0; i < impulseResponseLength; i++) {
		t_real t = -impulseResponseLength / 2 * samplingPeriod + i * samplingPeriod;
		t_real alpha = PI * t / symbolPeriod;
		if (0 == t) {
			impulseResponse[i] = (1 / symbolPeriod)*(1 + rollOffFactor * ((4 / PI) - 1));
		}
		else if (abs(t) == (symbolPeriod / (4 * rollOffFactor))) {
			impulseResponse[i] = (rollOffFactor / (symbolPeriod*sqrt(2)))*(1 + (2 / PI))*sin(PI / (4 * rollOffFactor)) + (1 - (2 / PI)*cos(PI / (4 * rollOffFactor)));
		}
		else {
			impulseResponse[i] = sin((alpha)*(1 - rollOffFactor)) + ((4 * rollOffFactor*t / symbolPeriod)*cos((alpha)*(1 + rollOffFactor)));
			impulseResponse[i] = (1 / symbolPeriod)* impulseResponse[i] / (alpha * (1 - ((4 * rollOffFactor * t / symbolPeriod) * (4 * rollOffFactor * t / symbolPeriod))));
		}
	}

	for (int i = 0; i < impulseResponseLength; i++)
	{
//		impulseResponse[i] = impulseResponse[i] / ((1 / symbolPeriod)*(1 + rollOffFactor * ((4 / PI) - 1)));
		
		if (normalizeEnergy) {
			impulseResponse[i] = impulseResponse[i] * samplingPeriod;       // Energy == 1
		}
		else {
			impulseResponse[i] = impulseResponse[i] * symbolPeriod;         // Amplitude == 1
		}


		gain = gain + impulseResponse[i];
	}

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

void square(vector<t_real> &impulseResponse, int impulseResponseLength, double samplingPeriod, double symbolPeriod) {

	int samplesPerSymbol = (int) (symbolPeriod / samplingPeriod);

	for (int k = 0; k < samplesPerSymbol; k++) {
		impulseResponse[k] = 1.0;
	};

	for (int k = samplesPerSymbol; k < impulseResponseLength; k++) {
		impulseResponse[k] = 0.0;
	};
};

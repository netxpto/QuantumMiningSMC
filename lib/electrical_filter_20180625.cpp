#include <algorithm>
#include <complex>
#include <array>
#include <vector>

#include "netxpto_20180418.h"
#include "electrical_filter_20180625.h"

using namespace std;


void ElectricalFilter::initialize(void) {

	double samplingPeriod = inputSignals[0]->getSamplingPeriod();
	double symbolPeriod = inputSignals[0]->getSymbolPeriod();
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());

	impulseResponseLength = (int)floor(impulseResponseTimeLength * symbolPeriod / samplingPeriod);

	impulseResponse.resize(impulseResponseLength);

	switch (getFilterType()) {

	case LowPass:
		lowPass(impulseResponse, impulseResponseLength, cutoffFrequency, samplingPeriod, symbolPeriod);
		break;
	case Defined:
		break;
	case Unitary:
		cutoffFrequency = 0;
		impulseResponseLength = symbolPeriod / samplingPeriod;
		impulseResponse.resize(impulseResponseLength);
		impulseResponse[0] = 1;
		for (int i = 1; i < symbolPeriod / samplingPeriod; i++) {
			impulseResponse[i] = 0;
		}
		break;
	};

	initializeFIR_Filter();

};

void ElectricalFilter::lowPass(vector<t_real> &impulseResponse, int impulseResponseLength, double cutoffFrequency, double samplingPeriod, double symbolPeriod) {
	double sinc;
	double gain{ 0 };

	window = getWindow(windowType, impulseResponseLength);

	for (int i = 0; i < impulseResponseLength; i++) {
		t_real t = -impulseResponseLength / 2 * samplingPeriod + i * samplingPeriod;
		if (t != 0) {
			sinc = sin(2 * PI * cutoffFrequency * t) / (2 * PI * cutoffFrequency * t);
		}
		else {
			sinc = 1;
		}
		impulseResponse[i] = sinc * 2 * cutoffFrequency;
		impulseResponse[i] = impulseResponse[i]*window[i];
		gain = gain + impulseResponse[i];
	};

	for (int i = 0; i < impulseResponseLength; i++)
	{
		impulseResponse[i] = impulseResponse[i] / gain;
	}
};
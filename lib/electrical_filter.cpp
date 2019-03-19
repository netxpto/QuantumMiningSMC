#include <algorithm>
#include <complex>
#include <array>
#include <vector>

#include "netxpto.h"
#include "electrical_filter.h"

using namespace std;

void lowPass(vector<t_real> &impulseResponse, int impulseResponseLength, double cutoffFrequency, double samplingPeriod, double symbolPeriod);

void ElectricalFilter::initialize(void) {

	double samplingPeriod = inputSignals[0]->samplingPeriod;
	double symbolPeriod = inputSignals[0]->symbolPeriod;

	impulseResponseLength = (int)floor(impulseResponseTimeLength * symbolPeriod / samplingPeriod);

	impulseResponse.resize(impulseResponseLength);

	switch (getFilterType()) {

	case LowPass:
		lowPass(impulseResponse, impulseResponseLength, cutoffFrequency, samplingPeriod, symbolPeriod);
		break;
	};

	initializeFIR_Filter();

};

void lowPass(vector<t_real> &impulseResponse, int impulseResponseLength, double cutoffFrequency, double samplingPeriod, double symbolPeriod) {
	double sinc;
	for (int i = 0; i < impulseResponseLength; i++) {
		t_real t = -impulseResponseLength / 2 * samplingPeriod + i * samplingPeriod;
		if (t != 0) {
			sinc = sin(2 * PI * cutoffFrequency * t) / (2 * PI * cutoffFrequency * t);
		}
		else {
			sinc = 1;
		}
		impulseResponse[i] = sinc * 2 * cutoffFrequency;
	};
};
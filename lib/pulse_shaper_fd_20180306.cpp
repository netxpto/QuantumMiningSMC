# include "netxpto.h"
# include "pulse_shaper_fd_20180306.h"

using namespace std;

void raisedCosineIR(vector<t_real> &impulseResponse, int impulseResponseLength, double rollOffFactor, double samplingPeriod, double symbolPeriod, bool passiveFilterMode);
void rootRaisedCosineIR(vector<t_real> &impulseResponse, int impulseResponseLength, double rollOffFactor, double samplingPeriod, double symbolPeriod, bool passiveFilterMode);
void gaussianIR(vector<t_real> &impulseResponse, int impulseResponseLength, double BTs, double samplingPeriod, double symbolPeriod, bool passiveFilterMode);
void squareIR(vector<t_real> &impulseResponse, int impulseResponseLength, double samplingPeriod, double symbolPeriod);


void PulseShaperFd::initialize(void) {

	double samplingPeriod = inputSignals[0]->samplingPeriod;
	double symbolPeriod = inputSignals[0]->symbolPeriod;

	impulseResponseLength = (int)floor(impulseResponseTimeLength * symbolPeriod / samplingPeriod);
	impulseResponse.resize(impulseResponseLength);

	symmetryTypeIr = getFilterSymmetryTypeIr();

	switch (getFilterType()) {

	case RaisedCosineIR:
		raisedCosineIR(impulseResponse, impulseResponseLength, rollOffFactor, samplingPeriod, symbolPeriod, passiveFilterMode);
		break;
	case RootRaisedCosineIR:
		rootRaisedCosineIR(impulseResponse, impulseResponseLength, rollOffFactor, samplingPeriod, symbolPeriod, passiveFilterMode);
		break;
	case GaussianIR:
		gaussianIR(impulseResponse, impulseResponseLength, getBTs(), samplingPeriod, symbolPeriod, passiveFilterMode);
		break;
	case SquareIR:
		squareIR(impulseResponse, impulseResponseLength, samplingPeriod, symbolPeriod);
		break;
	};

	initializeFD_Filter();
}

void raisedCosineIR(vector<t_real> &impulseResponse, int impulseResponseLength, double rollOffFactor, double samplingPeriod, double symbolPeriod, bool passiveFilterMode) {
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

	ofstream rcImpulseResponseReference("rcImpulseResponseReference.txt");
	rcImpulseResponseReference.precision(15);
	for (size_t i = 0; i < impulseResponse.size(); i++)
	{
		rcImpulseResponseReference << impulseResponse[i] << "\n";
	}
	rcImpulseResponseReference.close();
};

void rootRaisedCosineIR(vector<t_real> &impulseResponse, int impulseResponseLength, double rollOffFactor, double samplingPeriod, double symbolPeriod, bool passiveFilterMode)
{
	//double sinc;
	double gain{ 0 };
	for (int i = 0; i < impulseResponseLength; i++) {
		t_real t = -impulseResponseLength / 2 * samplingPeriod + i * samplingPeriod;
		/*if (t != 0) {
		sinc = sin(PI * t / symbolPeriod) / (PI * t / symbolPeriod);
		}
		else {
		sinc = 1;
		}*/
		if (t == 0)
		{
			impulseResponse[i] = (1 / symbolPeriod)*(1 + rollOffFactor*(4 / PI - 1));
		}
		else if (abs(t) == (symbolPeriod / (4 * rollOffFactor)))
		{
			impulseResponse[i] = (rollOffFactor / (symbolPeriod*sqrt(2)))*((1 + 2 / PI)*sin(PI / (4 * rollOffFactor)) + (1 - 2 / PI)*cos(PI / (4 * rollOffFactor)));
		}
		else
		{
			impulseResponse[i] = (1 / symbolPeriod)*((sin((1 - rollOffFactor)*PI*t / symbolPeriod)) + (4 * rollOffFactor*t / symbolPeriod)*(cos((1 + rollOffFactor)*PI*t / symbolPeriod))) * (1 / ((PI*t / symbolPeriod)*(1 - (4 * 4 * rollOffFactor*rollOffFactor*t*t / (symbolPeriod*symbolPeriod)))));

			//impulseResponse[i] = (4 * rollOffFactor)*(cos((1 + rollOffFactor)*PI*t / symbolPeriod) + (sin((1 - rollOffFactor)*PI*t / symbolPeriod)) / (4 * rollOffFactor*t / symbolPeriod)) * (1 / ((PI*sqrt(1))*(1 - (4 * 4 * rollOffFactor*rollOffFactor*t*t / (symbolPeriod*symbolPeriod)))));
		}
		gain = gain + impulseResponse[i];
	};

	double maxValue = *std::max_element(impulseResponse.begin(), impulseResponse.end());

	for (unsigned int i = 0; i < impulseResponse.size(); i++) {
		impulseResponse[i] = static_cast<double>((1 / maxValue)*impulseResponse[i]);
	}

	if (passiveFilterMode)
	{
		for (int i = 0; i < impulseResponseLength; i++)
		{
			impulseResponse[i] = impulseResponse[i] / gain;
		}
	}

	ofstream rcImpulseResponseReference("rrcImpulseResponseReference.txt");
	rcImpulseResponseReference.precision(15);
	for (size_t i = 0; i < impulseResponse.size(); i++)
	{
		rcImpulseResponseReference << impulseResponse[i] << "\n";
	}
	rcImpulseResponseReference.close();

}
void gaussianIR(vector<t_real> &impulseResponse, int impulseResponseLength, double BTs, double samplingPeriod, double symbolPeriod, bool passiveFilterMode) {
	/*double gauss;
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
	}*/

	double gauss;
	double gain{ 0 };
	t_real Ts = symbolPeriod;
	t_real T = samplingPeriod;
	t_real a = (sqrt(log(2) / 2) / BTs)*Ts;

	for (int i = 0; i < impulseResponseLength; i++)
	{
		t_real t = -impulseResponseLength / 2 * samplingPeriod + i * samplingPeriod;
		//gauss = (sqrt(PI)/a)*(exp(-(PI*t / a)*(PI*t / a)));
		gauss = (exp(-(PI*t / a)*(PI*t / a)));
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

void squareIR(vector<t_real> &impulseResponse, int impulseResponseLength, double samplingPeriod, double symbolPeriod) {

	int samplesPerSymbol = (int)(symbolPeriod / samplingPeriod);

	for (int k = 0; k < samplesPerSymbol; k++) {
		impulseResponse[k] = 1.0;
	};

	for (int k = samplesPerSymbol; k < impulseResponseLength; k++) {
		impulseResponse[k] = 0.0;
	};
};

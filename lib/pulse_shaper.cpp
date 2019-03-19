# include "netxpto.h"
# include "pulse_shaper.h"

using namespace std;

void raisedCosine(vector<t_real> &impulseResponse, int impulseResponseLength, double rollOffFactor, double samplingPeriod, double symbolPeriod, bool passiveFilterMode);
void rootRaisedCosine(vector<t_real> &impulseResponse, int impulseResponseLength, double rollOffFactor, double samplingPeriod, double symbolPeriod, bool passiveFilterMode);
void gaussian(vector<t_real> &impulseResponse, int impulseResponseLength, double BTs, double samplingPeriod, double symbolPeriod, bool passiveFilterMode);
void square(vector<t_real> &impulseResponse, int impulseResponseLength, double samplingPeriod, double symbolPeriod);

vector<t_real>  cheby_win(vector<t_real> out, int N, double atten);
double cheby_poly(int n, double x);

void PulseShaper::initialize(void) {

	double samplingPeriod = inputSignals[0]->samplingPeriod;
	double symbolPeriod = inputSignals[0]->symbolPeriod;

	impulseResponseLength = (int)floor(impulseResponseTimeLength * symbolPeriod / samplingPeriod);
	impulseResponse.resize(impulseResponseLength);

	switch (getFilterType()) {

		case RaisedCosine:
			raisedCosine(impulseResponse, impulseResponseLength, rollOffFactor, samplingPeriod, symbolPeriod, passiveFilterMode);
			break;
		case RootRaisedCosine:
			rootRaisedCosine(impulseResponse, impulseResponseLength, rollOffFactor, samplingPeriod, symbolPeriod, passiveFilterMode);
			break;
		case Gaussian:
			gaussian(impulseResponse, impulseResponseLength, getBTs(), samplingPeriod, symbolPeriod, passiveFilterMode);
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
	//////////////////////////////////////////////////////////////////////// different windows

	/*vector<t_real> window; // Hann
	for (int i = 0; i < impulseResponseLength; i++)
	{
		double val = 0.5 * (1 - cos(2 * PI*i / (impulseResponseLength - 1)));
		window.push_back(val);
	}*/

	/*vector<t_real> window; // Hamming
	for (int i = 0; i < impulseResponseLength; i++)
	{
		double val = 0.54 - 0.46*cos(2 * PI*i / (impulseResponseLength - 1));
		window.push_back(val);
	}*/

	/*vector<t_real> window; // Blackmann
	for (int i = 0; i < impulseResponseLength; i++)
	{
		double val = 0.42 - 0.5*cos(2 * PI*i / (impulseResponseLength - 1)) + 0.08*cos(4 * PI*i / (impulseResponseLength - 1));
		window.push_back(val);
	}*/

	/*vector<t_real> window; // Nuttall 
	for (int i = 0; i < impulseResponseLength; i++)
	{
		double a0 = 0.355768;
		double a1 = 0.487396;
		double a2 = 0.144232;
		double a3 = 0.012604;

	double val = a0- a1*cos(2 * PI*i / (impulseResponseLength - 1)) + a2*cos(4 * PI*i / (impulseResponseLength - 1))-a3*cos(6 * PI*i / (impulseResponseLength - 1));
	window.push_back(val);
	}*/

	vector<t_real> window; // Blackman-Nuttall
	for (int i = 0; i < impulseResponseLength; i++)
	{
	double a0 = 0.3635819;
	double a1 = 0.4891775;
	double a2 = 0.1365995;
	double a3 = 0.0106411;

	double val = a0- a1*cos(2 * PI*i / (impulseResponseLength - 1)) + a2*cos(4 * PI*i / (impulseResponseLength - 1))-a3*cos(6 * PI*i / (impulseResponseLength - 1));
	window.push_back(val);
	}
	
	/*vector<t_real> window(impulseResponseLength); // Blackman-Nuttall
	window = cheby_win(window, impulseResponseLength, 1000);
	*/

	///////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < impulseResponseLength; i++)
	{
		impulseResponse[i] = impulseResponse[i] * window[i];
	}

	// impulse response
	ofstream rcImpulseResponseReference("rcImpulseResponseReference.txt");
	rcImpulseResponseReference.precision(15);
	for (size_t i = 0; i < impulseResponse.size(); i++)
	{
		rcImpulseResponseReference << impulseResponse[i] << "\n";
	}
	rcImpulseResponseReference.close();

	// window
	ofstream windowReference("windowReference.txt");
	windowReference.precision(15);
	for (size_t i = 0; i < window.size(); i++)
	{
		windowReference << window[i] << "\n";
	}
	windowReference.close();
};

void rootRaisedCosine(vector<t_real> &impulseResponse, int impulseResponseLength, double rollOffFactor, double samplingPeriod, double symbolPeriod, bool passiveFilterMode)
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
			impulseResponse[i] = (1 / symbolPeriod)*((sin((1 - rollOffFactor)*PI*t / symbolPeriod)) + (4 * rollOffFactor*t / symbolPeriod)*(cos((1 + rollOffFactor)*PI*t / symbolPeriod))) * (1 / ((PI*t / symbolPeriod)*(1 - (4 * 4 * rollOffFactor*rollOffFactor*t*t / (symbolPeriod*symbolPeriod)))) );

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
void gaussian(vector<t_real> &impulseResponse, int impulseResponseLength, double BTs, double samplingPeriod, double symbolPeriod, bool passiveFilterMode) {
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
void square(vector<t_real> &impulseResponse, int impulseResponseLength, double samplingPeriod, double symbolPeriod) {

	int samplesPerSymbol = (int) (symbolPeriod / samplingPeriod);

	for (int k = 0; k < samplesPerSymbol; k++) {
		impulseResponse[k] = 1.0;
	};

	for (int k = samplesPerSymbol; k < impulseResponseLength; k++) {
		impulseResponse[k] = 0.0;
	};
};



vector<t_real>  cheby_win(vector<t_real> out1, int N, double atten)
{
	vector<t_real> out(N);
int nn, i;
double M, n, sum = 0, max = 0;
double tg = pow(10, atten / 20);  // 1/r term [2], 10^gamma [2]
double x0 = cosh( (1.0 / (N - 1))*acosh(tg) );
M = (N - 1) / 2;
if (N % 2 == 0) M = M + 0.5; // handle even length windows
for (nn = 0; nn<(N / 2 + 1); nn++)
{
n = nn - M;
sum = 0;
for (i = 1; i <= M; i++) {
sum += cheby_poly(N - 1, x0*cos(PI*i / N))*cos(2.0*n*PI*i / N);
}
out[nn] = tg + 2 * sum;
out[N - nn - 1] = out[nn];
if (out[nn]>max)max = out[nn];
}
for (nn = 0; nn<N; nn++) out[nn] /= max; // normalise everything
return out;
}

// **************************************************************************
// This function computes the chebyshev polyomial T_n(x)
// ***************************************************************************
double cheby_poly(int n, double x) {
double res;
if (fabs(x) <= 1) res = cos(n*acos(x));
else              res = cosh(n*acosh(x));
return res;
}
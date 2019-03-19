# include "netxpto.h"
# include "pulse_shaper_fd_20181110.h"
# include "overlap_save_20180208.h"
# include "fft_20180208.h"

using namespace std;

void raisedCosineTF(vector<t_complex> &transferFunction, int transferFunctionLength, double rollOffFactor, double samplingPeriod, double symbolPeriod);
void rootRaisedCosineTF(vector<t_complex> &transferFunction, int transferFunctionLength, double rollOffFactor, double samplingPeriod, double symbolPeriod);


void PulseShaperFd20181110::initialize(void)
{

	double samplingPeriod = inputSignals[0]->samplingPeriod;
	double symbolPeriod = inputSignals[0]->symbolPeriod;

	transferFunctionLength = getTransferFunctionLength();
	transferFunction.resize(transferFunctionLength);

	symmetryTypeTf = getFilterSymmetryTypeTf();

	switch (getFilterType())
	{

	case RaisedCosineTF:
		raisedCosineTF(transferFunction, getTransferFunctionLength(), rollOffFactor, samplingPeriod, symbolPeriod);
		break;

	case RootRaisedCosineTF:
		rootRaisedCosineTF(transferFunction, getTransferFunctionLength(), rollOffFactor, samplingPeriod, symbolPeriod);
		break;

	}
	initializeFD_Filter_20181110();

}

void raisedCosineTF(vector<t_complex> &transferFunction, int transferFunctionLength, double rollOffFactor, double samplingPeriod, double symbolPeriod) {

	t_real Ts = symbolPeriod;
	t_real T = samplingPeriod;
	t_real multipler = (Ts / T);

	t_real fWindow = 1 / samplingPeriod;
	t_real df = fWindow / transferFunctionLength;

	t_real f;
	vector<t_real> fSpan;
	for (int k = 0; k < transferFunctionLength; k++) 
	{
		f = -fWindow / 2 + k * df;
		fSpan.push_back(f);
	}

	vector<t_real> transferFunctionReal;
	for (unsigned int i = 0; i < fSpan.size(); i++)
	{
		// Calculate the transferFunctionAux 
		t_real negativeLimit = (1 - rollOffFactor) / (2 * Ts);
		t_real positiveLimit = (1 + rollOffFactor) / (2 * Ts);

		if (abs(fSpan[i]) <= negativeLimit)
		{
			transferFunctionReal.push_back(1* multipler);
		}
		else if (negativeLimit < abs(fSpan[i]) && abs(fSpan[i]) <= positiveLimit)
		{
			t_real value;
			value = multipler*0.5*(1 + cos((PI*Ts / rollOffFactor) * (abs(fSpan[i]) - ((1 - rollOffFactor) / (2 * Ts)))));
			transferFunctionReal.push_back(value);
		}
		else
		{
			transferFunctionReal.push_back(0);
		}
	}

	ofstream rcTf("rcTf.txt");
	rcTf.precision(15);
	for (size_t i = 0; i < transferFunctionReal.size(); i++)
	{
		rcTf << transferFunctionReal[i] << "\n";
	}
	rcTf.close();

	// convert transferFunctionAux to complex value
	vector<t_complex> transferFunctionComplex;
	vector<t_real> imTfn(transferFunctionReal.size());
	transferFunctionComplex = reImVect2ComplexVector(transferFunctionReal, imTfn);

	transferFunction = transferFunctionComplex;

}


void rootRaisedCosineTF(vector<t_complex> &transferFunction, int transferFunctionLength, double rollOffFactor, double samplingPeriod, double symbolPeriod) {

	t_real Ts = symbolPeriod;
	t_real T = samplingPeriod;
	t_real multipler = (Ts / T);

	t_real fWindow = 1 / samplingPeriod;
	t_real df = fWindow / transferFunctionLength;

	t_real f;
	vector<t_real> fSpan;
	for (int k = 0; k < transferFunctionLength; k++)
	{
		f = -fWindow / 2 + k * df;
		fSpan.push_back(f);
	}

	vector<t_real> transferFunctionReal;
	for (unsigned int i = 0; i < fSpan.size(); i++)
	{
		// Calculate the transferFunctionAux 
		t_real negativeLimit = (1 - rollOffFactor) / (2 * Ts);
		t_real positiveLimit = (1 + rollOffFactor) / (2 * Ts);

		if (abs(fSpan[i]) <= negativeLimit)
		{
			transferFunctionReal.push_back(1 * sqrt(multipler));
		}
		else if (negativeLimit < abs(fSpan[i]) && abs(fSpan[i]) <= positiveLimit)
		{
			t_real value;
			value = sqrt(multipler*0.5*(1 + cos((PI*Ts / rollOffFactor) * (abs(fSpan[i]) - ((1 - rollOffFactor) / (2 * Ts))))));
			transferFunctionReal.push_back(value);
		}
		else
		{
			transferFunctionReal.push_back(0);
		}
	}

	ofstream rrcTf("rrcTf.txt");
	rrcTf.precision(15);
	for (size_t i = 0; i < transferFunctionReal.size(); i++)
	{
		rrcTf << transferFunctionReal[i] << "\n";
	}
	rrcTf.close();

	// convert transferFunctionAux to complex value
	vector<t_complex> transferFunctionComplex;
	vector<t_real> imTfn(transferFunctionReal.size());
	transferFunctionComplex = reImVect2ComplexVector(transferFunctionReal, imTfn);

	transferFunction = transferFunctionComplex;

}


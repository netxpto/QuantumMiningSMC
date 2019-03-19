# include "netxpto.h"
# include "fir_filter.h"
# include "overlap_save_20180208.h"
# include "fft_20180208.h"

using namespace std;

void dcFilterTransferFunction(vector<t_complex> &transferFunction, int transferFunctionLength, double samplingPeriod, double symbolPeriod);
void hilbertFilterTransferFunction(vector<t_complex> &transferFunction, int transferFunctionLength, double samplingPeriod, double symbolPeriod);


void FirFilter::initialize(void)
{

	double samplingPeriod = inputSignals[0]->samplingPeriod;
	double symbolPeriod = inputSignals[0]->symbolPeriod;

	transferFunctionLength = 1024;
	transferFunction.resize(transferFunctionLength);

	switch (getFirFilterType())
	{

	case DcFilter:
		dcFilterTransferFunction(transferFunction, getTransferFunctionLength(), samplingPeriod, symbolPeriod);
		break;

	case HilberFilter:
		hilbertFilterTransferFunction(transferFunction, getTransferFunctionLength(), samplingPeriod, symbolPeriod);
		break;

	}
	initializeFD_Filter_20181110();

}

void dcFilterTransferFunction(vector<t_complex> &transferFunction, int transferFunctionLength, double samplingPeriod, double symbolPeriod)
{
	size_t length = transferFunctionLength;
	
	for (int i = 0; i < length; i++)
	{
		if (i == 0)
		{
			transferFunction[i] = { 0,0 };
		}
		else
		{
			transferFunction[i] = { 1,0 };
		}
	}
}

void hilbertFilterTransferFunction(vector<t_complex> &transferFunction, int transferFunctionLength, double samplingPeriod, double symbolPeriod)
{
	size_t length = transferFunctionLength;

	t_complex dcContent = { 0,0 };
	t_complex postiveFrequency = { 0,-1 };
	t_complex negativeFrequency = { 0,1 };

	for (int i = 0; i < length; i++)
	{
		if (i == 0)
		{
			transferFunction[i] = dcContent;

		}
		else if (i>0 && i<length / 2)
		{
			transferFunction[i] = postiveFrequency;

		}
		else
		{
			transferFunction[i] = negativeFrequency;
		}
	}
}





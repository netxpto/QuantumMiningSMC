#include "netxpto.h"
#include "transform.h"

using namespace std;

void Transform::initialize(void)
{
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->symbolPeriod);
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->samplingPeriod);
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->samplesPerSymbol);
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}

bool Transform::runBlock(void)
{

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();
	int process = min(ready, space);

	if (process <= 0) return false;

	t_real input;
	vector<double> inputBufferTimeDomainReal(process);

	vector<double> re(process);
	vector<double> im(process);
	vector<complex<double>> inputBufferTImeDomainComplex(process);

	vector<complex<double>> inputSignalFrequencyDomain(process);

	ComplexMult C;
	//FourierTransform FT;

	for (int i = 0; i < process; i++)					// Get the Input signal as a vector of size "n"
	{
		inputSignals[0]->bufferGet(&input);
		inputBufferTimeDomainReal.at(i) = input;
	}

	for (int i = 0; i < process; i++)
	{
		re[i] = inputBufferTimeDomainReal.at(i);			// Real part of input
		im[i] = 0;										// Imaginary part which is manipulated as "0"
	}

	C.ReImVect2ComplexVect(re, im, inputBufferTImeDomainComplex);					// Time domain complex form signal

	

	for (int i = 0; i < process; i++)					// Put the data using bufferput
	{
		t_real OUT;
		OUT = inputSignalFrequencyDomain[i].real();
		outputSignals[0]->bufferPut((t_real)(OUT));
	}




	return true;
}
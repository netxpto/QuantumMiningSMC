#include <algorithm>
#include <complex>
#include <chrono>
#include <fstream>

#include "netxpto.h"
#include "magToPhase.h"
#include <random>
# include "overlap_save_20180208.h"
# include "fft_20180208.h"

using namespace std;

void MagnitudeToPhase::initialize(void) {

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}


bool MagnitudeToPhase::runBlock(void) {

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();
	int process = min(ready, space);
	if (process == 0) return false;

	vector<t_complex> magnitudeData(process); // Get the Input signal
	t_real input;

	for (int i = 0; i < process; i++)
	{
		inputSignals[0]->bufferGet(&input);
		magnitudeData.at(i) = { sqrt(input), 0 };
	}

	int iterations = 50;
	vector<t_real> phase_data(process);
	vector<t_complex> A(process);
	vector<t_complex> B(process);
	vector<t_complex> C(process);
	vector<t_complex> D(process);
	vector<t_complex> out(process);

	for (int i = 0; i < iterations; i++)
	{

		for (int j = 0; j < process; j++)
		{
			A[j] = magnitudeData[j] * exp( complex<double>(0, phase_data[j]));
		}

		B = fft(A);

		for (int j1 = 0; j1 < process; j1++)
		{
			B[j1] = B[j1] / sqrt(process);
		}

		for (int k = 0; k < process; k++) // Causality
		{
			if (k == 0)
			{
				C[k] = {getDcValue(),0};
			}
			else if(k<process/2)
			{
				C[k] = { 0,0 };
			}
			else
			{
				C[k] = B[k];
			}
		}

		D = ifft(C);

		for (int k1 = 0; k1 < process; k1++)
		{
			D[k1] = D[k1] * sqrt(process);
		}

		for (int l = 0; l < process; l++) // Updated phase
		{
			phase_data[l] = arg(D[l]);
		}

		for (int l = 0; l < process; l++) // Recovered complex singal
		{
			out[l] = magnitudeData[l] * exp(complex<double>(0, phase_data[l]));
		}

	}

	for (int i = 0; i < process; i++) 
	{
		outputSignals[0]->bufferPut(t_complex(out[i]));
	}
			

	return true;
}

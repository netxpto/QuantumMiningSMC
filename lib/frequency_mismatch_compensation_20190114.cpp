# include "frequency_mismatch_compensation_20190114.h"

# include <algorithm>
# include <complex>
# include <iostream>
# include <fstream>
# include <random>
# include <numeric>
# include "fft_20180208.h"

using namespace std;


void FrequencyMismatchCompensation::initialize(void) {

	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
}


bool FrequencyMismatchCompensation::runBlock(void) {

	double symbolPeriod = inputSignals[0]->getSymbolPeriod();
	long int ready = inputSignals[0]->ready();
	long int space = outputSignals[0]->space();

	long int process = min(ready, space);

	if (process == 0) return false;
	t_complex in;
	t_complex out;
	vector<t_complex> xf(numberOfSamplesForEstimation);
	double xfReal;
	double averageAngle = 0;

	if (frequencyMode==PilotAided)
	{
		if ( !pilotRate )
		{
			cout << "ERRO: frequency_mismatch_compensation_20190114.cpp (PilotAided mode requires a pilotRate>0!)" << "\n";
			return false;
		}
		if (firstRun) 
		{
			int process0 = min(process, (numberOfSamplesForEstimation - 1)*(pilotRate)+1 - count);

			for (size_t i = 0; i < process0; i++) {
				inputSignals[0]->bufferGet(&in);
				delayLine[count] = in;
				if (count % pilotRate == 0)
				{
					samplesForEstimation[auxP] = in;
					auxP++;
				}
				count++;
			}
			if (process > process0) {
				firstRun = false;
				process = process - process0;

				xfReal = 0;
				for (size_t m = 0; m < numberOfSamplesForEstimation - 1; m++) {
					xf[m] = samplesForEstimation[m] * conj(samplesForEstimation[m + 1]);
					xfReal += real(xf[m]) / (numberOfSamplesForEstimation - 1);
				}
				frequencyEstimate = 0;
				if (xfReal < 0)
				{
					for (size_t i = 0; i < numberOfSamplesForEstimation - 1; i++) frequencyEstimate += (arg(xf[i] * -1.0) + PI) / ((numberOfSamplesForEstimation - 1) * pilotRate * symbolPeriod);
				}
				else
				{
					for (size_t i = 0; i < numberOfSamplesForEstimation - 1; i++) frequencyEstimate += (arg(xf[i])) / ((numberOfSamplesForEstimation - 1) * pilotRate * symbolPeriod);
				}

				for (size_t k = 0; k < (pilotRate)*(numberOfSamplesForEstimation - 1) + 1; k++)
				{
					theta = -frequencyEstimate * k * symbolPeriod;
					
					double phi = - frequencyEstimate * symbolPeriod;

					if ( pilotMode==Delay && k % 2) phi = 0;
					
					out = conj(delayLine[k]) * complex<double>(cos(theta+phi), sin(theta+phi));
					outputSignals[0]->bufferPut(out);
				}
			}
			else
			{
				return true;
			}
		}

		for (int i = 0; i < process; i++) 
		{
			inputSignals[0]->bufferGet(&in);
			if (count % pilotRate == 0) {
				rotate(samplesForEstimation.begin(), samplesForEstimation.begin() + 1, samplesForEstimation.end());
				samplesForEstimation[numberOfSamplesForEstimation - 1] = in;
				xfReal = 0;
				for (size_t m = 0; m < numberOfSamplesForEstimation - 1; m++) {
					xf[m] = samplesForEstimation[m] * conj(samplesForEstimation[m + 1]);
					xfReal += real(xf[m]) / (numberOfSamplesForEstimation - 1);
				}
				frequencyEstimate = 0;
				if (xfReal < 0)
				{
					for (size_t i = 0; i < numberOfSamplesForEstimation - 1; i++) frequencyEstimate += (arg(xf[i] * -1.0) + PI) / ((numberOfSamplesForEstimation - 1) * pilotRate * symbolPeriod);
				}
				else
				{
					for (size_t i = 0; i < numberOfSamplesForEstimation - 1; i++) frequencyEstimate += (arg(xf[i])) / ((numberOfSamplesForEstimation - 1) * pilotRate * symbolPeriod);
				}
			}
			theta = -frequencyEstimate * count * symbolPeriod;

			double phi = - frequencyEstimate * symbolPeriod;
			
			if (pilotMode == Delay && count % 2) phi = 0;

			out = conj(in) * complex<double>(cos(theta+phi), sin(theta+phi));
			outputSignals[0]->bufferPut(out);
			count++;
		}
	}
	else if (frequencyMode == BlindEstimation)
	{
		vector<double> FrequencyCoarse(1000);
		for (size_t i = 0; i < 1000; i++) FrequencyCoarse[i] = 1e7 + 1e7*i;

		vector<double> angleXfCoarse(numberOfSamplesForEstimation - 1);
		vector<double> angleXfFine(numberOfSamplesForEstimation - 1);
		double var;
		double varMin = 4 * PI*PI;
		int coarseIndex = 0;
		int fineIndex = 0;
		double frequencyEstimate = 0;

		if (firstRun) 
		{
			int process0 = min(process, (numberOfSamplesForEstimation - 1)*(pilotRate)+1 - count);

			for (size_t i = 0; i < process0; i++) {
				inputSignals[0]->bufferGet(&in);
				delayLine[count] = in;
				if (count % pilotRate == 0)
				{
					samplesForEstimation[auxP] = in;
					auxP++;
				}
				count++;
			}
			if (process > process0) 
			{
				firstRun = false;
				process = process - process0;
				
				for (size_t j = 0; j < 1000; j++)
				{
					double mean = 0;
					for (size_t k = 0; k < numberOfSamplesForEstimation-1; k++)
					{
						angleXfCoarse[k] = arg(xf[k] * polar(1.0, -FrequencyCoarse[j] * symbolPeriod*k));
						mean += angleXfCoarse[k] / (numberOfSamplesForEstimation - 1);
					}
					var = 0;
					for (size_t k = 0; k < numberOfSamplesForEstimation-1; k++) var += (angleXfCoarse[k] - mean) * (angleXfCoarse[k] - mean) / numberOfSamplesForEstimation;
					if (var < varMin) coarseIndex = j;
					varMin = min(varMin, var);
				}

				vector<double> FrequencyFine(1000);
				for (size_t i = 0; i < 1000; i++) {
					FrequencyFine[i] = FrequencyCoarse[coarseIndex] + 1e5*(i - 499.0);
					double a = 1e6 * (i - 499.0);
				}
				varMin = 4 * PI*PI;

				for (size_t j = 0; j < 1000; j++)
				{
					double mean = 0;
					for (size_t k = 0; k < numberOfSamplesForEstimation - 1; k++)
					{
						angleXfFine[k] = arg(xf[k] * polar(1.0, -FrequencyFine[j] * symbolPeriod*k));
						mean += angleXfFine[k] / (numberOfSamplesForEstimation - 1);
					}
					var = 0;
					for (size_t k = 0; k < numberOfSamplesForEstimation - 1; k++) var += (angleXfFine[k] - mean) * (angleXfFine[k] - mean) / numberOfSamplesForEstimation;
					if (var < varMin) fineIndex = j;
					varMin = min(varMin, var);
				}
				frequencyEstimate = FrequencyFine[fineIndex];
				for (size_t k = 0; k < (pilotRate)*(numberOfSamplesForEstimation - 1) + 1; k++)
				{
					theta = -frequencyEstimate * k * symbolPeriod;
					out = conj(delayLine[k]) * complex<double>(cos(theta), sin(theta));
					outputSignals[0]->bufferPut(out);
				}
			}
			else
			{
				return true;
			}
		}

		for (int i = 0; i < process; i++) {
			inputSignals[0]->bufferGet(&in);
			if (count % pilotRate == 0) {
				rotate(samplesForEstimation.begin(), samplesForEstimation.begin() + 1, samplesForEstimation.end());
				samplesForEstimation[numberOfSamplesForEstimation - 1] = in;

				for (size_t j = 0; j < 1000; j++)
				{
					double mean = 0;
					for (size_t k = 0; k < numberOfSamplesForEstimation - 1; k++)
					{
						angleXfCoarse[k] = arg(xf[k] * polar(1.0, -FrequencyCoarse[j] * symbolPeriod*k));
						mean += angleXfCoarse[k] / (numberOfSamplesForEstimation - 1);
					}
					var = 0;
					for (size_t k = 0; k < numberOfSamplesForEstimation - 1; k++) var += (angleXfCoarse[k] - mean) * (angleXfCoarse[k] - mean) / numberOfSamplesForEstimation;
					if (var < varMin) coarseIndex = j;
					varMin = min(varMin, var);
				}

				vector<double> FrequencyFine(1000);
				for (size_t i = 0; i < 1000; i++) {
					FrequencyFine[i] = FrequencyCoarse[coarseIndex] + 1e5*(i - 499.0);
					double a = 1e6 * (i - 499.0);
				}
				varMin = 4 * PI*PI;

				for (size_t j = 0; j < 1000; j++)
				{
					double mean = 0;
					for (size_t k = 0; k < numberOfSamplesForEstimation - 1; k++)
					{
						angleXfFine[k] = arg(xf[k] * polar(1.0, -FrequencyFine[j] * symbolPeriod*k));
						mean += angleXfFine[k] / (numberOfSamplesForEstimation - 1);
					}
					var = 0;
					for (size_t k = 0; k < numberOfSamplesForEstimation - 1; k++) var += (angleXfFine[k] - mean) * (angleXfFine[k] - mean) / numberOfSamplesForEstimation;
					if (var < varMin) fineIndex = j;
					varMin = min(varMin, var);
				}
				frequencyEstimate = FrequencyFine[fineIndex];
			}
			theta = -frequencyEstimate * count * symbolPeriod;
			double a = count * symbolPeriod;
			out = conj(in) * complex<double>(cos(theta), sin(theta));
			outputSignals[0]->bufferPut(out);
			count++;
		}
	}
	else if (frequencyMode == SpectralMethod)
	{
		if (pilotRate > 0)
		{
			vector<t_complex> xfFFT(numberOfSamplesForEstimation - 1);
			vector<t_complex> trash(numberOfSamplesForEstimation - 1);
			vector<double> amplitudeXfFFT(numberOfSamplesForEstimation - 1);

			if (firstRun)
			{
				int process0 = min(process, (numberOfSamplesForEstimation - 1)*(pilotRate)+1 - count);

				for (size_t i = 0; i < process0; i++) {
					inputSignals[0]->bufferGet(&in);
					delayLine[count] = in;
					if (count % pilotRate == 0)
					{
						samplesForEstimation[auxP] = in;
						auxP++;
					}
					count++;
				}
				if (process > process0) {
					firstRun = false;
					process = process - process0;

					xfFFT = fft(samplesForEstimation);
					trash = ifft(samplesForEstimation);

					double dW = PI / ((numberOfSamplesForEstimation - 1)*symbolPeriod);
					vector<double> W(numberOfSamplesForEstimation - 1);
					for (size_t i = 0; i < numberOfSamplesForEstimation - 1; i++)
					{
						W[i] = -(numberOfSamplesForEstimation - 1) / 2 / 2 * dW + i * dW;
						amplitudeXfFFT[i] = abs(xfFFT[i]);
					}


					int maxElementeIndex = max_element(amplitudeXfFFT.begin(), amplitudeXfFFT.end()) - amplitudeXfFFT.begin();
					frequencyEstimate = W[maxElementeIndex] / 5; //  8885765876.316732; //
					for (size_t k = 0; k < (pilotRate)*(numberOfSamplesForEstimation - 1) + 1; k++)
					{
						theta = -frequencyEstimate * k * symbolPeriod;
						out = conj(delayLine[k]) * complex<double>(cos(theta), sin(theta));
						outputSignals[0]->bufferPut(out);
					}
				}
				else
				{
					return true;
				}
			}


			for (int i = 0; i < process; i++)
			{
				inputSignals[0]->bufferGet(&in);
				if (count % pilotRate == 0) {
					rotate(samplesForEstimation.begin(), samplesForEstimation.begin() + 1, samplesForEstimation.end());
					samplesForEstimation[numberOfSamplesForEstimation - 1] = in;

					xfFFT = fft(samplesForEstimation);
					trash = ifft(samplesForEstimation);

					double dW = PI / ((numberOfSamplesForEstimation - 1)*symbolPeriod);
					vector<double> W(numberOfSamplesForEstimation - 1);
					for (size_t i = 0; i < numberOfSamplesForEstimation - 1; i++)
					{
						W[i] = -(numberOfSamplesForEstimation - 1) / 2 / 2 * dW + i * dW;
						amplitudeXfFFT[i] = abs(xfFFT[i]);
					}

					int maxElementeIndex = max_element(amplitudeXfFFT.begin(), amplitudeXfFFT.end()) - amplitudeXfFFT.begin();
					frequencyEstimate = W[maxElementeIndex] / 5;
				}
				theta = -frequencyEstimate * count * symbolPeriod;
				double a = count * symbolPeriod;
				out = conj(in) * complex<double>(cos(theta), sin(theta));
				outputSignals[0]->bufferPut(out);
				count++;
			}
		}
		else
		{
			vector<t_complex> xfFFT(numberOfSamplesForEstimation);
			vector<t_complex> trash(numberOfSamplesForEstimation);
			vector<double> amplitudeXfFFT(numberOfSamplesForEstimation);

			if (firstRun)
			{
				int process0 = min(process, numberOfSamplesForEstimation - count);

				for (size_t i = 0; i < process0; i++) {
					inputSignals[0]->bufferGet(&in);
					delayLine[count] = in;
					samplesForEstimation[count] = in * in * in * in ;
					count++;
				}
				if (process > process0) {
					firstRun = false;
					process = process - process0;

					xfFFT = fft(samplesForEstimation);
					trash = ifft(samplesForEstimation);

					double dW = PI / ((numberOfSamplesForEstimation)*symbolPeriod);
					vector<double> W(numberOfSamplesForEstimation);
					for (size_t i = 0; i < numberOfSamplesForEstimation - 1; i++)
					{
						W[i] = -(numberOfSamplesForEstimation - 1) / 2 / 2 * dW + i * dW;
						amplitudeXfFFT[i] = abs(xfFFT[i]);
					}


					int maxElementeIndex = max_element(amplitudeXfFFT.begin(), amplitudeXfFFT.end()) - amplitudeXfFFT.begin();
					frequencyEstimate = W[maxElementeIndex] / 4; //  8885765876.316732; //
					for (size_t k = 0; k < numberOfSamplesForEstimation; k++)
					{
						theta = frequencyEstimate * k * symbolPeriod;
						out = conj(delayLine[k]) * complex<double>(cos(theta), sin(theta));
						t_complex f = complex<double>(cos(theta), sin(theta));
						outputSignals[0]->bufferPut(out);
					}
				}
				else
				{
					return true;
				}
			}


			for (int i = 0; i < process; i++)
			{
				inputSignals[0]->bufferGet(&in);
				
				rotate(samplesForEstimation.begin(), samplesForEstimation.begin() + 1, samplesForEstimation.end());
				samplesForEstimation[numberOfSamplesForEstimation - 1] = in * in * in * in ;
				
				//xfFFT = fft(samplesForEstimation);
				//trash = ifft(samplesForEstimation);

/*				double dW = PI / ((numberOfSamplesForEstimation - 1)*symbolPeriod);
				vector<double> W(numberOfSamplesForEstimation - 1);
				for (size_t i = 0; i < numberOfSamplesForEstimation - 1; i++)
				{
					W[i] = -(numberOfSamplesForEstimation - 1) / 2 / 2 * dW + i * dW;
					amplitudeXfFFT[i] = abs(xfFFT[i]);
				}

				int maxElementeIndex = max_element(amplitudeXfFFT.begin(), amplitudeXfFFT.end()) - amplitudeXfFFT.begin();
				frequencyEstimate = W[maxElementeIndex] / 4;*/
				
				theta = -frequencyEstimate * count * symbolPeriod;
				double a = count * symbolPeriod;
				out = conj(in) * complex<double>(cos(theta), sin(theta));
				outputSignals[0]->bufferPut(out);
				count++;
			}
		}
	}

	return true;
}

#include <algorithm>
#include <complex>
#include "fft_20180208.h"
#include "netxpto_20180118.h"
#include "psd_estimator_20180704.h"
#include <typeinfo>
#include <stdexcept>

//// Currently estimating SNR only for real valued electrical signals.
//// Does not work for SNR< 10dB. Results also dubious for SNR>35;
//// Needs improvement.



void PSDEstimator::initialize(void) {
	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	if (numberOfInputSignals == 2 && numberOfOutputSignals == 2) {
		outputSignals[1]->setSymbolPeriod(inputSignals[1]->getSymbolPeriod());
		outputSignals[1]->setSamplingPeriod(inputSignals[1]->getSamplingPeriod());
		outputSignals[1]->setFirstValueToBeSaved(inputSignals[1]->getFirstValueToBeSaved());
	}
}

bool PSDEstimator::runBlock(void) {

	double inValueI;
	double inValueQ;
	complex<double> inValue;
	double samplingPeriod = inputSignals[0]->getSamplingPeriod();
	double symbolPeriod = inputSignals[0]->getSymbolPeriod();
	int sps = inputSignals[0]->getSamplesPerSymbol();
	vector<double> segment;
	vector <double> im(segmentSize);
	vector <complex<double>> segmentComplex(segmentSize);
	vector <complex<double>> fourierTransformed;
	double noisePower = 0;
	double signalPower = 0;
	double SNR = 0;
	double stdSNR = 0;
	double LowerBound;
	double UpperBound;

	// If its the first pass, calculate z for finding the confidence interval.
	// Also, calculate the window to be used and the vector of frequencies.
	if (firstPass) {
		firstPass = false;
		double x1 = -2;
		double x2 = 2;
		double x3 = x2 - (erf(x2 / sqrt(2)) + 1 - alpha)*(x2 - x1) / (erf(x2 / sqrt(2)) - erf(x1 / sqrt(2)));
		double exactness = 1e-15;
		while (abs(erf(x3 / sqrt(2)) + 1 - alpha) > exactness)
		{
			x3 = x2 - (erf(x2 / sqrt(2)) + 1 - alpha)*(x2 - x1) / (erf(x2 / sqrt(2)) - erf(x1 / sqrt(2)));
			x1 = x2;
			x2 = x3;
		}
		z = -x3;

		window = getWindow(windowType, segmentSize);
		
		U = 0;	// Window normalization constant
		for (unsigned int i = 0; i < window.size(); i++) {
			U += pow(window[i], 2);
		}
		U = U / window.size();
		
		for (int i = 0; i < segmentSize; i++) {
			frequencies.insert(frequencies.end(), ((1 + i - segmentSize / 2) * 1 / samplingPeriod)/segmentSize);
		}
	}
	
	int ready;
	int space;

	if (numberOfInputSignals == 2 && numberOfOutputSignals == 2) {
		ready = min(inputSignals[0]->ready(), inputSignals[1]->ready());
		space = min(outputSignals[0]->space(), outputSignals[1]->space());
	} else {
		ready = inputSignals[0]->ready();
		space = outputSignals[0]->space();
	}

	int available = min(ready, space);
	int process = min(available, measuredIntervalSize - (int)measuredInterval.size());
	
	if (available == 0) {
		return false;
	}
	
	
	// Get values until the vector/array has the desired number of elements
	for (long int i = 0; i < process; i++) {
		if (inputSignals[0]->getValueType() == ComplexValue) {
			inputSignals[0]->bufferGet(&inValue);
			outputSignals[0]->bufferPut(inValue);
			if (ignoreInitialSamples>0) {
				ignoreInitialSamples -= 1;
			}
			if (ignoreInitialSamples == 0) {
				measuredInterval.insert(measuredInterval.end(), inValue);
			}
		} else {
			inputSignals[0]->bufferGet(&inValueI);
			outputSignals[0]->bufferPut(inValueI);
			if (ignoreInitialSamples>0) {
				ignoreInitialSamples -= 1;
			} if (numberOfInputSignals == 2 && numberOfOutputSignals == 2) {
				inputSignals[1]->bufferGet(&inValueQ);
				outputSignals[1]->bufferPut(inValueQ);
				inValue= complex<double>(inValueI, inValueQ);
			} else {
				inValue=complex<double>(inValueI,0);
			} if (ignoreInitialSamples == 0) {
				measuredInterval.insert(measuredInterval.end(), inValue);
			}
		}
	}
	
	if (measuredInterval.size() == measuredIntervalSize) {
		switch (method) {
			// Get Welch's PSD estimate
			case WelchPgram:
			{
				int start = 0;
				int finish = start + segmentSize;
				vector<double> periodogram;
				int summed = 0;
				vector<double> periodogramTmp(segmentSize);
				while (finish < (int)measuredInterval.size()) {
					// Create segment from full interval
					segmentComplex.assign(measuredInterval.begin() + start, measuredInterval.begin() + finish);

					// Multiply by window (in time domain)
					for (long int i = 0; i < segmentSize; i++) {
						segmentComplex[i] *= window[i];
					}

					// Create imaginary vector and generate complex segment
					/*if (numberOfInputSignals == 1) {
						for (unsigned int i = 0; i < segment.size(); i++)
						{
							// Imaginary data of the signal
							im[i] = 0;
						}
						segmentComplex = ReImVect2ComplexVector(segment, im);
					}*/
					// Get the FFT
					fourierTransformed = fft(segmentComplex);
//					fourierTransformed = fftshift(fourierTransformed);
					double absFFTi = 0;
					for (unsigned int i = 0; i < fourierTransformed.size(); i++) {
						absFFTi = abs(fourierTransformed[i]);
						periodogramTmp[i] = pow(absFFTi, 2) / (U*segmentSize);
					}

					// Add to summing vector
					if (totalPeriodogramSum.empty()) {
						totalPeriodogramSum = periodogramTmp;
					} else {
						for (unsigned int i = 0; i < periodogramTmp.size(); i++) {
							totalPeriodogramSum[i] += periodogramTmp[i];
						}
					}

					totalSummed += 1;
					segment.clear();
					//periodogramTmp.clear();
					fourierTransformed.clear();
					segmentComplex.clear();
					start += segmentSize - overlapCount;
					finish += segmentSize - overlapCount;
				}

				// Divide by number of segments used, and separate the signal frequencies from the rest
				periodogram = totalPeriodogramSum;
				for (unsigned int i = 0; i < periodogramTmp.size(); i++) {
					periodogram[i] = totalPeriodogramSum[i] / totalSummed;
				}


			/* Outputting a .txt report*/
				ofstream outputFile;
				outputFile.open(filename);
	//			myfile.open("SNR.txt", std::ios_base::app);
				outputFile << "PSD estimate\n";
				outputFile << "Frequencies\n";
				for(int idx = 0; idx < frequencies.size(); idx++){
					outputFile << frequencies[idx] << "\t";
				}
				outputFile << "\nValues\n";
				for(int idx = 0; idx < periodogram.size(); idx++){
					outputFile << periodogram[idx] << "\t";
				}
//					myfile << "Upper and lower confidence bounds for " << (1 - alpha) * 100 << "% confidence level \n";
//					myfile << "Upper Bound= " << UpperBound << "\n";
//					myfile << "Lower Bound= " << LowerBound << "\n";
//				outputFile << "\nNumber of measurements= " << totalSummed.size() << "\n";
				outputFile.close();
				measuredInterval.clear();
				break;
			}
			}
	}
	
	return true;
}


/*vector<double> PSDEstimator::getWindow(WindowType windowType, int windowSize) {
	vector<double> wn(windowSize);
	switch (windowType)
	{
		case Hamming:
			for (int x = 0; x < windowSize; x++) {
				wn[x] = 0.54 - 0.46*cos(2 * PI*x / (windowSize - 1));
			}
			return wn;

		case Hanning:
			for (int x = 0; x < windowSize; x++) {
				wn[x] = 0.5 *(1 - cos(2 * PI*x / (windowSize - 1)));
			}
			return wn;
	}
	return wn;
}*/

// NOT YET INCLUDED IN THE FFT ".CPP" file. REMOVE WHEN IT ARRIVES
vector<complex<double>> PSDEstimator::fftshift(vector<complex<double>> &vec) {
	unsigned long long N = vec.size();
	vector<complex<double>> output;

	if (N % 2 == 0)
	{
		for (unsigned long long i = N / 2; i < vec.size(); i++)
		{
			output.push_back(vec[i]);
		}

		for (unsigned long long i = 0; i < N / 2; i++)
		{
			output.push_back(vec[i]);
		}
	}
	else
	{
		N = N + 1;
		for (unsigned long long i = N / 2; i < vec.size(); i++)
		{
			output.push_back(vec[i]);
		}

		for (unsigned long long i = 0; i < N / 2; i++)
		{
			output.push_back(vec[i]);
		}

	}

	return output;

}

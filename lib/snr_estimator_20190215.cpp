#include <algorithm>
#include <complex>
#include "fft_20180208.h"
#include "netxpto_20190215.h"
#include "snr_estimator_20190215.h"
#include <typeinfo>
#include <stdexcept>

//// Currently estimating SNR only for real valued electrical signals.
//// Does not work for SNR< 10dB. Results also dubious for SNR>35;
//// Needs improvement.



void SNREstimator::initialize(void) {
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	if (numberOfInputSignals == 2 && numberOfOutputSignals == 2) {
		outputSignals[1]->setSymbolPeriod(inputSignals[1]->getSymbolPeriod());
		outputSignals[1]->setSamplingPeriod(inputSignals[1]->getSamplingPeriod());
		outputSignals[1]->setFirstValueToBeSaved(inputSignals[1]->getFirstValueToBeSaved());
	}
}

bool SNREstimator::runBlock(void) {

	double inValueI;
	double inValueQ;
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
	complex<double> inValue;

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
	
	if (active) {
		// Get values until the vector/array has the desired number of elements
		for (long int i = 0; i < process; i++) {
			if (inputSignals[0]->getValueType() == ComplexValue) {
				inputSignals[0]->bufferGet(&inValue);
				outputSignals[0]->bufferPut(inValue);
				if (ignoreInitialSamples > 0) {
					ignoreInitialSamples -= 1;
				}
				if (ignoreInitialSamples == 0) {
					measuredInterval.insert(measuredInterval.end(), inValue);
				}
			}
			else {
				inputSignals[0]->bufferGet(&inValueI);
				outputSignals[0]->bufferPut(inValueI);
				if (ignoreInitialSamples > 0) {
					ignoreInitialSamples -= 1;
				} if (numberOfInputSignals == 2 && numberOfOutputSignals == 2) {
					inputSignals[1]->bufferGet(&inValueQ);
					outputSignals[1]->bufferPut(inValueQ);
					inValue = complex<double>(inValueI, inValueQ);
				}
				else {
					inValue = complex<double>(inValueI, 0);
				} if (ignoreInitialSamples == 0) {
					measuredInterval.insert(measuredInterval.end(), inValue);
				}
			}
		}

		if (measuredInterval.size() == measuredIntervalSize) {
			switch (method) {
			case powerSpectrum:
			{
				// Get Welch's PSD estimate
				int start = 0;
				int finish = start + segmentSize;
				int summed = 0;
				vector<double> periodogramSum;
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
					if (inputSignals[0]->getValueType() != ComplexValue) {
						fourierTransformed = fftshift(fourierTransformed);
					}
					double absFFTi = 0;
					for (unsigned int i = 0; i < fourierTransformed.size(); i++) {
						absFFTi = abs(fourierTransformed[i]);
						periodogramTmp[i] = pow(absFFTi, 2) / (U*segmentSize);
					}

					// Add to summing vector
					if (periodogramSum.empty()) {
						periodogramSum = periodogramTmp;
					}
					else {
						for (unsigned int i = 0; i < periodogramTmp.size(); i++) {
							periodogramSum[i] += periodogramTmp[i];
						}
					}
					summed += 1;
					segment.clear();
					//periodogramTmp.clear();
					fourierTransformed.clear();
					segmentComplex.clear();
					start += segmentSize - overlapCount;
					finish += segmentSize - overlapCount;
				}

				vector<double> signalPsd;
				vector<double> noisePsd;
				vector<double> noiseBorders;
				double noiseBorderPower = 0;
				// Divide by number of segments used, and separate the signal frequencies from the rest
				for (unsigned int i = 0; i < periodogramTmp.size(); i++) {
					periodogramSum[i] = periodogramSum[i] / summed;
					if ((frequencies[i] < 0.5*(1 + rollOffComp) / symbolPeriod) && (frequencies[i] > -0.5*(1 + rollOffComp) / symbolPeriod)) {
						signalPsd.insert(signalPsd.end(), periodogramSum[i]);
					}
					else if (((frequencies[i] > 0.5*(1 + rollOffComp) / symbolPeriod) && (frequencies[i] < noiseBw)) || ((frequencies[i] < -0.5*(1 + rollOffComp) / symbolPeriod) && (frequencies[i] > -noiseBw))) {
						noisePsd.insert(noisePsd.end(), periodogramSum[i]);
					}
					else {
						noiseBorders.insert(noiseBorders.end(), periodogramSum[i]);
					}
				}

				// Calculate overall noise and signal power
				for (unsigned int i = 0; i < noisePsd.size(); i++) {
					noisePower += (noisePsd[i]);
				}

				for (unsigned int i = 0; i < signalPsd.size(); i++) {
					signalPower += (signalPsd[i]);
				}

				for (unsigned int i = 0; i < noiseBorders.size(); i++) {
					noiseBorderPower += (noiseBorders[i]);
				}

				signalPower = signalPower - noisePower * (1 + rollOffComp)*(1 / symbolPeriod) / (noisePsd.size()*(1 / samplingPeriod) / periodogramSum.size());
				noisePower = noisePower * 2 * noiseBw / (noisePsd.size()*(1 / samplingPeriod) / periodogramSum.size()) + noiseBorderPower;
				if (signalPower <= 0) {
					cout << "ERROR: SNR too low, cannot identify the signal within the noise" << "\n";
				}
				else {
					allSNR.insert(allSNR.end(), signalPower / noisePower);
					cout << "SNR: " << 10 * (log10(signalPower) - log10(noisePower)) << "\n";
				}

				/* Calculating average SNR and bounds */
				if (!allSNR.empty()) {


					for (unsigned int i = 0; i < allSNR.size(); i++) {
						SNR += allSNR[i];
					}
					SNR = SNR / allSNR.size();
					for (unsigned int i = 0; i < allSNR.size(); i++) {
						stdSNR += pow((SNR - allSNR[i]), 2);
					}
					if (allSNR.size() > 1) {
						stdSNR = sqrt(stdSNR / (allSNR.size() - 1));
					}
					else {
						stdSNR = 0;
					}

					if (allSNR.size() > 1) {
						LowerBound = 10 * log10(SNR - z * stdSNR / sqrt(allSNR.size()));
						UpperBound = 10 * log10(SNR + z * stdSNR / sqrt(allSNR.size()));
					}
					else {
						LowerBound = 0;
						UpperBound = 0;
					}
					SNR = 10 * log10(SNR);
					//			if (LowerBound<lowestMinorant) {
					//					LowerBound = lowestMinorant;
					//				}

							/* Outputting a .txt report*/
					ofstream myfile;
					myfile.open(filename);
					//			myfile.open("SNR.txt", std::ios_base::app);
					myfile << "SNR= " << SNR << "\n";
					myfile << "Upper and lower confidence bounds for " << (1 - alpha) * 100 << "% confidence level \n";
					myfile << "Upper Bound= " << UpperBound << "\n";
					myfile << "Lower Bound= " << LowerBound << "\n";
					myfile << "Number of measurements= " << allSNR.size() << "\n";
					myfile.close();
				}
				measuredInterval.clear();
				noisePower = 0;
				signalPower = 0;
				break;
			}
			case constantAmplitudeMoments:
			{
				// Does not work yet
				int start = 0;
				int finish = start + segmentSize;
				int summed = 0;

				vector<complex<double>> sampledSegment;

				for (int i = 0; i < measuredInterval.size(); i++)
				{
					if (i % sps == 0) {
						sampledSegment.insert(sampledSegment.end(), measuredInterval[i]);
					}
				}

				// Get fourth power and its fourth root
				for (int i = 0; i < sampledSegment.size(); i++)
				{
					sampledSegment[i] = pow(sampledSegment[i], 4);
					sampledSegment[i] = pow(sampledSegment[i], 1 / 4);
				}


				// Get average and variance -> Signal == average, noise == variance
				for (unsigned int i = 0; i < sampledSegment.size(); i++) {
					signalPower += real(sampledSegment[i]);
				}
				signalPower = signalPower / sampledSegment.size();
				for (unsigned int i = 0; i < sampledSegment.size(); i++) {
					noisePower += pow((signalPower - real(sampledSegment[i])), 2);
				}
				if (sampledSegment.size() > 1) {
					noisePower = sqrt(noisePower / (sampledSegment.size() - 1));
				}
				else {
					noisePower = 0;
				}

				if (signalPower <= 0) {
					cout << "ERROR: SNR too low, cannot identify the signal within the noise" << "\n";
				}
				else {
					allSNR.insert(allSNR.end(), signalPower / noisePower);
					cout << "SNR: " << 10 * (log10(signalPower) - log10(noisePower)) << "\n";
				}

				/* Calculating average SNR and bounds */
				if (!allSNR.empty()) {


					for (unsigned int i = 0; i < allSNR.size(); i++) {
						SNR += allSNR[i];
					}
					SNR = SNR / allSNR.size();
					for (unsigned int i = 0; i < allSNR.size(); i++) {
						stdSNR += pow((SNR - allSNR[i]), 2);
					}
					if (allSNR.size() > 1) {
						stdSNR = sqrt(stdSNR / (allSNR.size() - 1));
					}
					else {
						stdSNR = 0;
					}

					if (allSNR.size() > 1) {
						LowerBound = 10 * log10(SNR - z * stdSNR / sqrt(allSNR.size()));
						UpperBound = 10 * log10(SNR + z * stdSNR / sqrt(allSNR.size()));
					}
					else {
						LowerBound = 0;
						UpperBound = 0;
					}
					SNR = 10 * log10(SNR);
					//			if (LowerBound<lowestMinorant) {
					//					LowerBound = lowestMinorant;
					//				}

					/* Outputting a .txt report*/
					ofstream myfile;
					myfile.open(filename);
					//			myfile.open("SNR.txt", std::ios_base::app);
					myfile << "SNR= " << SNR << "\n";
					myfile << "Upper and lower confidence bounds for " << (1 - alpha) * 100 << "% confidence level \n";
					myfile << "Upper Bound= " << UpperBound << "\n";
					myfile << "Lower Bound= " << LowerBound << "\n";
					myfile << "Number of measurements= " << allSNR.size() << "\n";
					myfile.close();
				}
				measuredInterval.clear();
				noisePower = 0;
				signalPower = 0;
				break;
				// Identify the symbols (points of constant amplitude)

				// Get the average of the absolute value (square root of signal power)

				// Get noise power by subtracting to average signal power (signal variance)

				// Divide signal power by noise power, get log and do stuff for interval
			}
			case m2m4:
			{

				int start = 0;
				int finish = start + segmentSize;
				int summed = 0;
				double ones = 0;
				double zeros = 0;
				double stdOnes = 0;
				double stdZeros = 0;

				vector<complex<double>> sampledSegment;
				vector<double> m2Vec;
				vector<double> m4Vec;
				vector<double> m4NewVec;
				double m2 = 0;
				double m4 = 0;
				double m4New = 0;

				for (int i = 0; i < measuredInterval.size(); i++)
				{
					if (i % sps == 0) {
						sampledSegment.insert(sampledSegment.end(), measuredInterval[i]);
					}
				}
				// HERE the binary list should be checked to find ones and zeros
				for (int i = 0; i < sampledSegment.size(); i++)
				{
					m2Vec.insert(m2Vec.end(), pow(real(sampledSegment[i]), 2) + pow(imag(sampledSegment[i]), 2));
					m4Vec.insert(m4Vec.end(), pow(pow(real(sampledSegment[i]), 2) + pow(imag(sampledSegment[i]), 2), 2));
					m4NewVec.insert(m4NewVec.end(), pow(pow(imag(sampledSegment[i]), 2) - pow(real(sampledSegment[i]), 2), 2));
				}

				for (unsigned int i = 0; i < m2Vec.size(); i++) {
					m2 += m2Vec[i];
					m4 += m4Vec[i];
					m4New += m4NewVec[i];
				}
				m2 = m2 / m2Vec.size();
				m4 = m4 / m4Vec.size();
				m4New = m4New / m4NewVec.size();

				signalPower = 2 * pow(m2, 2);
				signalPower = signalPower - m4;
				signalPower = pow(signalPower, 0.5);

				noisePower = (m2 - pow(2 * pow(m2, 2) - m4, 0.5));


				if ((signalPower <= 0) || (isnan(signalPower))) {
					cout << "ERROR: SNR too low, cannot identify the signal within the noise" << "\n";
				}
				else {
					allSNR.insert(allSNR.end(), signalPower / noisePower);
					cout << "SNR: " << 10 * (log10(signalPower) - log10(noisePower)) << "\n";
				}

				/* Calculating average SNR and bounds */
				if (!allSNR.empty()) {


					for (unsigned int i = 0; i < allSNR.size(); i++) {
						SNR += allSNR[i];
					}
					SNR = SNR / allSNR.size();
					for (unsigned int i = 0; i < allSNR.size(); i++) {
						stdSNR += pow((SNR - allSNR[i]), 2);
					}
					if (allSNR.size() > 1) {
						stdSNR = sqrt(stdSNR / (allSNR.size() - 1));
					}
					else {
						stdSNR = 0;
					}

					if (allSNR.size() > 1) {
						LowerBound = 10 * log10(SNR - z * stdSNR / sqrt(allSNR.size()));
						UpperBound = 10 * log10(SNR + z * stdSNR / sqrt(allSNR.size()));
					}
					else {
						LowerBound = 0;
						UpperBound = 0;
					}
					SNR = 10 * log10(SNR);
					//			if (LowerBound<lowestMinorant) {
					//					LowerBound = lowestMinorant;
					//				}

					/* Outputting a .txt report*/
					ofstream myfile;
					myfile.open(filename);
					//			myfile.open("SNR.txt", std::ios_base::app);
					myfile << "SNR= " << SNR << "\n";
					myfile << "Upper and lower confidence bounds for " << (1 - alpha) * 100 << "% confidence level \n";
					myfile << "Upper Bound= " << UpperBound << "\n";
					myfile << "Lower Bound= " << LowerBound << "\n";
					myfile << "Number of measurements= " << allSNR.size() << "\n";
					myfile.close();
				}
				measuredInterval.clear();
				noisePower = 0;
				signalPower = 0;
				break;
				// I think it requires complex signal
				break;
			}
			case ren:
			{

				int start = 0;
				int finish = start + segmentSize;
				int summed = 0;
				double ones = 0;
				double zeros = 0;
				double stdOnes = 0;
				double stdZeros = 0;

				vector<complex<double>> sampledSegment;
				vector<double> m2Vec;
				vector<double> m4Vec;
				vector<double> m4NewVec;
				double m2 = 0;
				double m4New = 0;

				for (int i = 0; i < measuredInterval.size(); i++)
				{
					if (i % sps == 0) {
						sampledSegment.insert(sampledSegment.end(), measuredInterval[i]);
					}
				}
				// HERE the binary list should be checked to find ones and zeros
				for (int i = 0; i < sampledSegment.size(); i++)
				{
					m2Vec.insert(m2Vec.end(), pow(real(sampledSegment[i]), 2) + pow(imag(sampledSegment[i]), 2));
					m4NewVec.insert(m4NewVec.end(), pow(pow(imag(sampledSegment[i]), 2) - pow(real(sampledSegment[i]), 2), 2));
				}

				for (unsigned int i = 0; i < m2Vec.size(); i++) {
					m2 += m2Vec[i];
					m4New += m4NewVec[i];
				}
				m2 = m2 / m2Vec.size();
				m4New = m4New / m4NewVec.size();

				/*				signalPower = 2 * pow(m2, 2);
				signalPower = signalPower - m4;
				signalPower = pow(signalPower,0.5);

				noisePower = (m2 - pow(2 * pow(m2, 2) - m4, 0.5));
				*/
				signalPower = pow(m2, 2) - m4New;
				signalPower = pow(signalPower, 0.5);

				noisePower = pow(m2, 2);
				noisePower = noisePower - m4New;
				noisePower = (m2 - pow(noisePower, 0.5));

				if ((signalPower <= 0) || (isnan(signalPower))) {
					cout << "ERROR: SNR too low, cannot identify the signal within the noise" << "\n";
				}
				else {
					allSNR.insert(allSNR.end(), signalPower / noisePower);
					cout << "SNR: " << 10 * (log10(signalPower) - log10(noisePower)) << "\n";
				}

				/* Calculating average SNR and bounds */
				if (!allSNR.empty()) {


					for (unsigned int i = 0; i < allSNR.size(); i++) {
						SNR += allSNR[i];
					}
					SNR = SNR / allSNR.size();
					for (unsigned int i = 0; i < allSNR.size(); i++) {
						stdSNR += pow((SNR - allSNR[i]), 2);
					}
					if (allSNR.size() > 1) {
						stdSNR = sqrt(stdSNR / (allSNR.size() - 1));
					}
					else {
						stdSNR = 0;
					}

					if (allSNR.size() > 1) {
						LowerBound = 10 * log10(SNR - z * stdSNR / sqrt(allSNR.size()));
						UpperBound = 10 * log10(SNR + z * stdSNR / sqrt(allSNR.size()));
					}
					else {
						LowerBound = 0;
						UpperBound = 0;
					}
					SNR = 10 * log10(SNR);
					//			if (LowerBound<lowestMinorant) {
					//					LowerBound = lowestMinorant;
					//				}

					/* Outputting a .txt report*/
					ofstream myfile;
					myfile.open(filename);
					//			myfile.open("SNR.txt", std::ios_base::app);
					myfile << "SNR= " << SNR << "\n";
					myfile << "Upper and lower confidence bounds for " << (1 - alpha) * 100 << "% confidence level \n";
					myfile << "Upper Bound= " << UpperBound << "\n";
					myfile << "Lower Bound= " << LowerBound << "\n";
					myfile << "Number of measurements= " << allSNR.size() << "\n";
					myfile.close();
				}
				measuredInterval.clear();
				noisePower = 0;
				signalPower = 0;
				break;
				// I think it requires complex signal
				break;
			}
			case qFactor:
			{

				// This was hacked quickly. Should resort to original binary sequence to identify ones and zeros.
				// Should provide a good aproximation as is, though.
				int start = 0;
				int finish = start + segmentSize;
				int summed = 0;
				double ones = 0;
				double zeros = 0;
				double stdOnes = 0;
				double stdZeros = 0;

				vector<complex<double>> sampledSegment;
				vector<double> sampledSegmentOnes;
				vector<double> sampledSegmentZeros;

				for (int i = 0; i < measuredInterval.size(); i++)
				{
					if (i % sps == 0) {
						sampledSegment.insert(sampledSegment.end(), measuredInterval[i]);
					}
				}
				// HERE the binary list should be checked to find ones and zeros
				for (int i = 0; i < sampledSegment.size(); i++)
				{
					if (real(sampledSegment[i]) > 0)
					{
						sampledSegmentOnes.insert(sampledSegmentOnes.end(), real(sampledSegment[i]));
					}
					else {
						sampledSegmentZeros.insert(sampledSegmentZeros.end(), real(sampledSegment[i]));
					}
				}
				for (int i = 0; i < sampledSegment.size(); i++)
				{
					if (imag(sampledSegment[i]) > 0)
					{
						sampledSegmentOnes.insert(sampledSegmentOnes.end(), imag(sampledSegment[i]));
					}
					else {
						sampledSegmentZeros.insert(sampledSegmentZeros.end(), imag(sampledSegment[i]));
					}
				}



				for (unsigned int i = 0; i < sampledSegmentOnes.size(); i++) {
					ones += sampledSegmentOnes[i];
				}
				ones = ones / sampledSegmentOnes.size();
				for (unsigned int i = 0; i < sampledSegmentZeros.size(); i++) {
					zeros += sampledSegmentZeros[i];
				}
				zeros = zeros / sampledSegmentOnes.size();

				for (unsigned int i = 0; i < sampledSegmentOnes.size(); i++) {
					stdOnes += pow((ones - sampledSegmentOnes[i]), 2);
				}
				if (sampledSegmentOnes.size() > 1) {
					stdOnes = sqrt(stdOnes / (sampledSegmentOnes.size() - 1));
				}
				else {
					stdOnes = 0;
				}

				for (unsigned int i = 0; i < sampledSegmentZeros.size(); i++) {
					stdZeros += pow((zeros - sampledSegmentZeros[i]), 2);
				}
				if (sampledSegmentZeros.size() > 1) {
					stdZeros = sqrt(stdZeros / (sampledSegmentZeros.size() - 1));
				}
				else {
					stdZeros = 0;
				}

				// Get average and variance -> Signal == average, noise == variance

				signalPower = ones - zeros;

				noisePower = stdOnes + stdZeros;

				if (signalPower <= 0) {
					cout << "ERROR: SNR too low, cannot identify the signal within the noise" << "\n";
				}
				else {
					allSNR.insert(allSNR.end(), signalPower / noisePower);
					cout << "SNR: " << 10 * (log10(signalPower) - log10(noisePower)) << "\n";
				}

				/* Calculating average SNR and bounds */
				if (!allSNR.empty()) {


					for (unsigned int i = 0; i < allSNR.size(); i++) {
						SNR += allSNR[i];
					}
					SNR = SNR / allSNR.size();
					for (unsigned int i = 0; i < allSNR.size(); i++) {
						stdSNR += pow((SNR - allSNR[i]), 2);
					}
					if (allSNR.size() > 1) {
						stdSNR = sqrt(stdSNR / (allSNR.size() - 1));
					}
					else {
						stdSNR = 0;
					}

					if (allSNR.size() > 1) {
						LowerBound = 10 * log10(SNR - z * stdSNR / sqrt(allSNR.size()));
						UpperBound = 10 * log10(SNR + z * stdSNR / sqrt(allSNR.size()));
					}
					else {
						LowerBound = 0;
						UpperBound = 0;
					}
					SNR = 10 * log10(SNR);
					//			if (LowerBound<lowestMinorant) {
					//					LowerBound = lowestMinorant;
					//				}

					/* Outputting a .txt report*/
					ofstream myfile;
					myfile.open(filename);
					//			myfile.open("SNR.txt", std::ios_base::app);
					myfile << "SNR= " << SNR << "\n";
					myfile << "Upper and lower confidence bounds for " << (1 - alpha) * 100 << "% confidence level \n";
					myfile << "Upper Bound= " << UpperBound << "\n";
					myfile << "Lower Bound= " << LowerBound << "\n";
					myfile << "Number of measurements= " << allSNR.size() << "\n";
					myfile.close();
				}
				measuredInterval.clear();
				noisePower = 0;
				signalPower = 0;
				break;
			}
			}
		}
	} else {

		for (long int i = 0; i < process; i++) {
			if (inputSignals[0]->getValueType() == ComplexValue) {
				inputSignals[0]->bufferGet(&inValue);
				outputSignals[0]->bufferPut(inValue);
			} else {
				inputSignals[0]->bufferGet(&inValueI);
				outputSignals[0]->bufferPut(inValueI);
				if (numberOfInputSignals == 2 && numberOfOutputSignals == 2) {
					inputSignals[1]->bufferGet(&inValueQ);
					outputSignals[1]->bufferPut(inValueQ);
				}
			}
		}
}
	return true;
}


/*vector<double> SNREstimator::getWindow(WindowType windowType, int windowSize) {
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

// NOT YET INCLUDED IN THE FFT ".CPP". REMOVE WHEN IT ARRIVES
vector<complex<double>> SNREstimator::fftshift(vector<complex<double>> &vec)
{
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

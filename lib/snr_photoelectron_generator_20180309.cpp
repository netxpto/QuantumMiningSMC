#include <algorithm>
#include <complex>

#include "netxpto_20180118.h"
#include "snr_photoelectron_generator_20180309.h"

void SnrPhotoelectronGenerator::initialize(void){
	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(tWindow);
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	
	// Average n samples per time window
	double samplingPeriod = inputSignals[0]->getSamplingPeriod();
	averageNSamplesPerWindow = tWindow/samplingPeriod;

	// First Interval Calculation
	lastWindowSample = (long)ceil(averageNSamplesPerWindow*(currentWindowIndex + 1));

	// Number of samples of the first timeWindow
	nWindowSamples = lastWindowSample;

}


bool SnrPhotoelectronGenerator::runBlock(void){

	// If tWindow is 0.0, then we will consider an infinite time window.

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();
	int process = min(ready, space);

	
	if (process == 0)
	{
		return false;
	}
	
	double signalValue;
	double SNR;
	long int remainingWindowSamples = 0;
	int short_process = 0;

	while(true){

		if ((currentWindowSample == nWindowSamples) && (tWindow>0.0)){

			//// Debuging ////
			//cout << "Calculating SNR\n";
			//cout << currentWindowIndex << endl;

			// SNR Calculation: (estimated average)/(estimated variance)
			SNR = (double)nWindowSamples/((double)nWindowSamples-1) *
			      pow(aux_sum1,2)/(nWindowSamples*aux_sum2 - pow(aux_sum1,2));

			// Write output signal
			// DEBUG
			//cout << "SNR\t" << SNR << endl;
			outputSignals[0]->bufferPut(SNR);
						
			// Auxiliary sums reset.
			aux_sum1 = 0.0;
			aux_sum2 = 0.0;

			// Current window sample counter reset
			currentWindowSample = 0;

			// Calculation of SNR window limits.
			currentWindowIndex ++;
			firstWindowSample = lastWindowSample;
			lastWindowSample  = (long)ceil(averageNSamplesPerWindow*(currentWindowIndex+1));
			nWindowSamples = lastWindowSample - firstWindowSample;
		}

		if (process == 0){
			return true;
		}

		if (tWindow>0.0){
			remainingWindowSamples = nWindowSamples - currentWindowSample;
			short_process = min((long int)process, remainingWindowSamples);
		}else {
			short_process = process;
		}
				
		for(int i=0; i<short_process; i++){

			// Obtain value from input signal
			inputSignals[0]->bufferGet(&signalValue);
			
			// Add to the moments
			aux_sum1 += signalValue;
			aux_sum2 += signalValue*signalValue;

			// DEBUG
			//cout << aux_sum1 << "\t" << aux_sum2 << endl;
		}
		
		//// Debuging ////
		//cout << "process: " << process << "\tremainingWindowSamples: " << remainingWindowSamples << endl;

		process             -= short_process;
		currentWindowSample += short_process;

	}
}



// This function will override the default "terminate()" function in the netxpto block definition
void SnrPhotoelectronGenerator::terminate(void){
	
	// DEBUG
	/*
	cout << "TERMINATOR" << endl;
	cout << "Calculating final SNR\n";
	*/

	double SNR;

	// Number of samples processed until this moment.
	long int nWindowSamples = currentWindowSample;

	// DEBUG
	/*
	cout << "currentWindowSample: " << currentWindowSample << endl;
	cout << "aux1: " << aux_sum1 << endl;
	cout << "aux2: " << aux_sum2 << endl;
	cout << "currentWindowIndex: " << currentWindowIndex << endl;
	cout << "first: " << firstWindowSample << endl;
	cout << "last: "  << lastWindowSample  << endl;
	*/

	// If no samples were summed, exit.
	if (nWindowSamples == 0) return;

	// SNR Calculation: (estimated average)/(estimated variance)
	SNR = (double)nWindowSamples / ((double)nWindowSamples - 1) *
	      pow(aux_sum1, 2) / (nWindowSamples*aux_sum2 - pow(aux_sum1, 2));

	outputSignals[0]->bufferPut(SNR);
}

#include <algorithm>
#include <complex>
#include <chrono>
#include <fstream>

#include "netxpto.h"
#include "hilbert_transformer.h"
#include <random>
# include "overlap_save_20180208.h"
# include "fft_20180208.h"

using namespace std;

void HilbertTransform::initialize(void) {

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}


bool HilbertTransform::runBlock(void) {

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();
	int process = min(ready, space);
	if (process == 0) return false;

	
		// put code here

		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////
		t_complex input;
		vector<t_complex> currentCopy(process);
		for (int i = 0; i < process; i++)
		{
			inputSignals[0]->bufferGet(&input);
			currentCopy.at(i) = input;
		}

		// Hilbert Transform Transformer Impulse Response

		int span = (hilbertTransferImpulseResponseLength)/2;
		vector<t_complex> hn(hilbertTransferImpulseResponseLength);

		for (int i = -span; i <= span; i++)
		{
			if (i % 2 == 0)
				hn.at(i + span) = { 0,0 };
			else
				hn.at(i + span) = { 2/(i*PI),0 };
		}


		// Apply overlap and save method
		vector<t_complex> y;
		vector<t_complex> previousCopy; 		vector<t_complex> previousCopyVal(process);


		if (K == 0){ previousCopy = previousCopyVal; }

		vector<t_complex> OUTaux = overlapSave(currentCopy, previousCopy, hn);

		previousCopy = currentCopy;
		K = K + 1;

		// Remove the size modified data (opppsite to "currentCopyAux to currentCopy")
		vector<t_complex> OUT;
		for (int i = 0; i < process; i++) {
			OUT.push_back(OUTaux[previousCopy.size() + i]);
		}

		// Bufferput
		for (int i = 0; i < process; i++) {
			t_real val;
			val = OUT[i].real();
			outputSignals[0]->bufferPut((t_real)(val));
		}

	return true;
}

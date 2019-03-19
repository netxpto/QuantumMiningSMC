#include <algorithm>
#include <complex>
#include <fstream>

#include "netxpto.h"
#include "dc_filter.h"
#include "fft_20180208.h"


void DcFilter::initialize(void) {

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}


bool DcFilter::runBlock(void) {

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process == 0) return false;

	signal_value_type sTypeIn = inputSignals[0]->getValueType();
	signal_value_type sTypeOut = outputSignals[0]->getValueType();

	if (sTypeIn != sTypeOut)
	{
		cout << "ERRO: add.cpp (signal type mismatch! (Signal must be the same type))" << "\n";
		return false;
	}

	vector<t_complex> input;
	t_complex in;
	for (int i=0; i<process; i++){
		inputSignals[0]->bufferGet(&in);
		input.push_back(in);
	}

	vector<t_complex> inputFft;
	inputFft = input;
	fft(inputFft);
	inputFft[0] = 0;
	
	vector<t_complex> output;

	output = ifft(inputFft);

	for (int i = 0; i < process; i++) 
		outputSignals[0]->bufferPut(t_complex(output[i]));


	return true;
}

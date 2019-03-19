#include <algorithm>
#include <complex>
#include <iostream>
#include <fstream>

#include "netxpto.h"
#include "testblock.h"


void testblock::initialize(void){

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	 
	outputSignals[0]->setCentralWavelength(inputSignals[0]->getCentralWavelength());
	outputSignals[0]->setCentralFrequency(inputSignals[0]->getCentralWavelength());

}


bool testblock::runBlock(void){

	ofstream myfile1;
	myfile1.open("y.txt", fstream::app);

	int ready = inputSignals[0]->ready();

	int space = outputSignals[0]->space();
	
	int process = min(ready, space);

	if (process == 0){
		myfile1.close();
		return false;
	}

	complex<t_real> imaginary(0, 1);
	
	for (int i = 0; i < process; i++) {

		t_real in;
		t_real out;
		inputSignals[0]->bufferGet(&in);

		out = in*1e6;

		outputSignals[0]->bufferPut(out);
		myfile1 << out << "\n";
	}
	return true;
}

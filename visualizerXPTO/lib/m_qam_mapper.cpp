# include <complex>
# include <fstream>
# include <iostream>
# include <math.h>
# include <stdio.h>
# include <string>
# include <strstream>
# include <vector>
# include <algorithm>

# include "netplus.h"
# include "m_qam_mapper.h"


using namespace std;

/*
MQamMapper::MQamMapper(vector<Signal *> &InputSig, vector<Signal *> &OutputSig):Block(InputSig, OutputSig) {
	
	2016-08-03
	numberOfInputSignals = InputSig.size();
	numberOfOutputSignals = OutputSig.size();

	inputSignals = InputSig;
	outputSignals = OutputSig;
	
	outputSignals[0]->symbolPeriod = 2 * inputSignals[0]->symbolPeriod;
	outputSignals[0]->samplingPeriod = 2 * inputSignals[0]->samplingPeriod;

	outputSignals[1]->symbolPeriod = 2 * inputSignals[0]->symbolPeriod;
	outputSignals[1]->samplingPeriod = 2 * inputSignals[0]->samplingPeriod;

	setM(m);
	
	

	
}*/

void MQamMapper::initialize(void){

	outputSignals[0]->symbolPeriod = 2 * inputSignals[0]->symbolPeriod;
	outputSignals[0]->samplingPeriod = 2 * inputSignals[0]->samplingPeriod;
	outputSignals[0]->samplesPerSymbol = 1;
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	outputSignals[1]->symbolPeriod = 2 * inputSignals[0]->symbolPeriod;
	outputSignals[1]->samplingPeriod = 2 * inputSignals[0]->samplingPeriod;
	outputSignals[1]->samplesPerSymbol = 1;
	outputSignals[1]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	setM(m);
}

bool MQamMapper::runBlock(void) {


	int ready = inputSignals[0]->ready();
	int space1 = outputSignals[0]->space();
	int space2 = outputSignals[1]->space();

	int space = (space1 <= space2) ? space1 : space2;
	int length = (ready <= (2 * space)) ? ready : space; // equivalent to min(ready, 2 * space);

	if (length <= 0) return false;

	int binaryValue;
	int nBinaryValues = (int)log2(m);
	for (int i = 0; i < length; i++) {
		inputSignals[0]->bufferGet(&binaryValue);
		auxSignalNumber = auxSignalNumber + (int) pow(2, nBinaryValues - 1 - auxBinaryValue) * binaryValue;
		auxBinaryValue++;
		if (auxBinaryValue == nBinaryValues) {
			t_real auxI = iqAmplitudes[auxSignalNumber].i;
			t_real auxQ = iqAmplitudes[auxSignalNumber].q;
			outputSignals[0]->bufferPut((t_real)auxI);
			outputSignals[1]->bufferPut((t_real)auxQ);
			auxBinaryValue = 0;
			auxSignalNumber = 0;
		}
	}

	return true;
}

void MQamMapper::setIqAmplitudes(vector<t_iqValues> iqAmplitudesValues){
	m = iqAmplitudesValues.size();
	iqAmplitudes.resize(m);
	iqAmplitudes = iqAmplitudesValues; 
};

void MQamMapper::setM(int mValue){
	m = mValue;

	iqAmplitudes.resize(m);
	switch (m) {
	case 4:
		iqAmplitudes = { { 1.0, 1.0 }, { -1.0, 1.0 }, { -1.0, -1.0 }, { 1.0, -1.0 } };
	};
};
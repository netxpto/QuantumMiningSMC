# include "m_qam_mapper_20180118.h"


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

	outputSignals[0]->setSymbolPeriod(log2(m) * inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(log2(m) * inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(1);
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	outputSignals[1]->setSymbolPeriod(log2(m) * inputSignals[0]->getSymbolPeriod());
	outputSignals[1]->setSamplingPeriod(log2(m) * inputSignals[0]->getSamplingPeriod());
	outputSignals[1]->setSamplesPerSymbol(1);
	outputSignals[1]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	//setM(m);
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
			t_real auxI = iqAmplitudes[auxSignalNumber].real();
			t_real auxQ = iqAmplitudes[auxSignalNumber].imag();
			outputSignals[0]->bufferPut((t_real)auxI);
			outputSignals[1]->bufferPut((t_real)auxQ);
			auxBinaryValue = 0;
			auxSignalNumber = 0;
		}
	}

	return true;
}

void MQamMapper::setIqAmplitudes(vector<t_iqValues> iqAmplitudesValues){
	m = (int) iqAmplitudesValues.size();
	iqAmplitudes.resize(m);
	iqAmplitudes = iqAmplitudesValues; 
};

void MQamMapper::setM(int mValue){
	m = mValue;

	iqAmplitudes.resize(m);
	switch (m) {
	case 4:
		iqAmplitudes = { { 1.0, 1.0 }, { -1.0, 1.0 }, { -1.0, -1.0 }, { 1.0, -1.0 } };

	case 16:
		iqAmplitudes = { { -3.0, -3.0 },{ -3.0, -1.0 },{ -3.0, 3.0 },{ -3.0, 1.0 },{ -1.0, -3.0 },{ -1.0, -1.0 },{ -1.0, 3.0 },{ -1.0, 1.0 },{ 3.0, -3.0 },{ 3.0, -1.0 }, { 3.0, 3.0 },{ 3.0, 1.0 },{ 1.0, -3.0 },{ 1.0, -1.0 },{ 1.0, 3.0 },{ 1.0, 1.0 } };
	};
}; 

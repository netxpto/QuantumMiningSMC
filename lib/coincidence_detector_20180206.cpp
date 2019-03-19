#include "coincidence_detector_20180206.h"
/*Outputing a .txt with information about the bits sent by Alice and the bits received by Bob for each case*/
/*
CoincidenceDetector::~CoincidenceDetector() {

	

	ofstream myfile;
	myfile.open(fileName);
	myfile << "Number of Bits sent by Alice: " << bitsAlice << "\n";
	myfile << "Number of Bits received by Bob: " << bitsAlice - bitsAtten -bitsDoubl << "\n";
	myfile << "Number of Bits double click: " << bitsDoubl << " \n";
	myfile << "Number of Bits that suffered attenuation: " << bitsAtten << " \n";

	myfile.close();

	return;
}
*/
void CoincidenceDetector::initialize(void) {

	outputSignals[0]->setSymbolPeriod(inputSignals[1]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[1]->getSamplingPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[1]->getSamplesPerSymbol());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[1]->getFirstValueToBeSaved());
}

bool CoincidenceDetector::runBlock(void) {

	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();
	int ready = min(ready0, ready1);

	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process <= 0) return false;

	for (auto k = 0; k < process; k++) {
		t_binary value0;
		inputSignals[0]->bufferGet(&value0);

		t_binary value1;
		inputSignals[1]->bufferGet(&value1);

		bitsAlice++;

		if ((value0 == 1) && (value1 == 0)) {
			outputSignals[0]->bufferPut((t_real)0.0);
		}
		else if ((value0 == 0) && (value1 == 1)) {
			outputSignals[0]->bufferPut((t_real)1.0);
		}
		else if ((value0 == 1) && (value1 == 1)) {
			outputSignals[0]->bufferPut((t_real)-2.0);
			bitsDoubl++;
		}
		else {
			outputSignals[0]->bufferPut((t_real)-1.0);
			bitsAtten++;
		}
		
	}

	return true;
};

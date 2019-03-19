# include "sinkstring.h"
#include <iostream>
#include <fstream>

using namespace std;
string outString = "";

SinkString::SinkString(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) {

  numberOfInputSignals = (int) InputSig.size();
  numberOfOutputSignals = (int) OutputSig.size();

  inputSignals = InputSig;
}

bool SinkString::runBlock(void)
{

	int ready = inputSignals[0]->ready();

	int process;
	if (numberOfSamples >= 0) {
		process = std::min((long int)ready, numberOfSamples);
	}
	else {
		process = ready;
	}
	 
	if ((process == 0) || (numberOfSamples==0)) return false;

/*	if ((inputSignals[0])->getType().compare("TimeDiscreteAmplitudeDiscreteReal"))
		for (int i = 0; i<process; i++) static_cast<TimeDiscreteAmplitudeDiscreteReal *>(inputSignals[0])->bufferGet();

	if ((inputSignals[0])->getType().compare("Binary"))
		for (int i = 0; i<process; i++) static_cast<Binary *>(inputSignals[0])->bufferGet();

	if ((inputSignals[0])->getType().compare("TimeDiscreteAmplitudeContinuousReal"))
		for (int i = 0; i<process; i++) static_cast<TimeDiscreteAmplitudeContinuousReal *>(inputSignals[0])->bufferGet();

	if ((inputSignals[0])->getType().compare("TimeDiscreteAmplitudeContinuousComplex"))
	for (int i = 0; i<process; i++) static_cast<TimeDiscreteAmplitudeContinuousComplex *>(inputSignals[0])->bufferGet();

	if ((inputSignals[0])->getType().compare("TimeContinuousAmplitudeDiscreteReal"))
		for (int i = 0; i<process; i++) static_cast<TimeContinuousAmplitudeDiscreteReal *>(inputSignals[0])->bufferGet();

	if ((inputSignals[0])->getType().compare("TimeContinuousAmplitudeContinuousComplex"))
		for (int i = 0; i<process; i++) static_cast<TimeContinuousAmplitudeContinuousComplex *>(inputSignals[0])->bufferGet();*/
	
	//char letter;
	int bitOut[8];
	char output = 0;
	ofstream myfile;
	

	//ifstream reader("textOut.txt");
	//if (reader) {
	//	do{
	//		reader.get(letter);
	//		outString = outString + letter;
	//	} while (!reader.eof());
	//	reader.close();
	//}

	for (int j = 0; j < process; j=j+8) {
		for (int i = 0; i < 8; ++i) {
			inputSignals[0]->bufferGet(&bitOut[i]);
			output = (int)output + (int)(bitOut[i] * pow(2, i));
		}
		
		outString = outString + output;
		output = 0;
	}
	
	myfile.open("textOut.txt");
	myfile << outString;
	myfile.close();

	numberOfSamples = numberOfSamples - process;
	if (displayNumberOfSamples) cout << numberOfSamples << "\n";

	return true;
}

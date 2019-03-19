# include <complex>
# include <fstream>
# include <iostream>
# include <math.h>
# include <stdio.h>
# include <string>
# include <strstream>
# include <vector>
# include <algorithm> 
# include <random>
# include "netxpto.h"
# include "binary_source.h"

using namespace std;


/*2016-08-03
BinarySource::BinarySource(vector<Signal*> &InputSig, vector<Signal*> &OutputSig) {

  numberOfInputSignals = InputSig.size();
  numberOfOutputSignals = OutputSig.size();

  inputSignals = InputSig;
  outputSignals = OutputSig;

  outputSignals[0]->symbolPeriod = bitPeriod;
  outputSignals[0]->samplingPeriod = outputSignals[0]->symbolPeriod;
  
}*/

void BinarySource::initialize(void){
	numberOfOutputSignals = outputSignals.size();

	for (auto i = 0; i < numberOfOutputSignals; ++i) {
		outputSignals[i]->setSymbolPeriod(bitPeriod);
		outputSignals[i]->setSamplingPeriod(outputSignals[i]->getSymbolPeriod());
		outputSignals[i]->setSamplesPerSymbol(1);
		outputSignals[i]->setFirstValueToBeSaved(1);
	}
}

bool BinarySource::runBlock(void) {

	long int space = outputSignals[0]->space();
	for (auto k = 1; k < numberOfOutputSignals; k++) {
		long int aux = outputSignals[k]->space();
		space = std::min(space, aux);
	}
	
	long int process;
	if (numberOfBits >= 0) {
		process = std::min( space, numberOfBits);
	}
	else {
		process = space;
	}

	if (process <= 0) return false;

	if (mode == PseudoRandom){

		if (acumul.size() == 0) {

			acumul.resize(33);

			for (int i = 0; i < 32; i += 2) {
				acumul[i] = 1;
				acumul[i + 1] = 0;
			}
			acumul[5] = 0;
			acumul[4] = 0;
			acumul[3] = 1;
			acumul[2] = 0;
			acumul[1] = 1;
			acumul[0] = 0;
		}

		int len = patternLength;

		vector<int>& ac = acumul;

		for (int k = 0; k < process; k++) {
			t_binary aux = (t_binary)ac[len];
			for (auto l = 0; l < numberOfOutputSignals; ++l) {
				outputSignals[l]->bufferPut((t_binary)aux);
			}
			numberOfBits--;

			for (int i = len; i > 0; --i) ac[i] = ac[i - 1];

			switch (len) {
			case 1:
				ac[0] = ac[1];
				break;
			case 2:
				ac[0] = (ac[2] + ac[1]) % 2;
				break;
			case 3:
				ac[0] = (ac[3] + ac[1]) % 2;
				break;
			case 4:
				ac[0] = (ac[4] + ac[1]) % 2;
				break;
			case 5:
				ac[0] = (ac[5] + ac[2]) % 2;
				break;
			case 6:
				ac[0] = (ac[6] + ac[1]) % 2;
				break;
			case 7:
				ac[0] = (ac[7] + ac[1]) % 2;
				break;
			case 8:
				ac[0] = (ac[8] + ac[4] + ac[3] + ac[2]) % 2;
				break;
			case 9:
				ac[0] = (ac[9] + ac[4]) % 2;
				break;
			case 10:
				ac[0] = (ac[10] + ac[3]) % 2;
				break;
			case 11:
				ac[0] = (ac[11] + ac[2]) % 2;
				break;
			case 12:
				ac[0] = (ac[12] + ac[6] + ac[4] + ac[1]) % 2;
				break;
			case 13:
				ac[0] = (ac[13] + ac[4] + ac[3] + ac[1]) % 2;
				break;
			case 14:
				ac[0] = (ac[14] + ac[5] + ac[3] + ac[1]) % 2;
				break;
			case 15:
				ac[0] = (ac[15] + ac[1]) % 2;
				break;
			case 16:
				ac[0] = (ac[16] + ac[5] + ac[3] + ac[2]) % 2;
				break;
			case 17:
				ac[0] = (ac[17] + ac[3]) % 2;
				break;
			case 18:
				ac[0] = (ac[18] + ac[5] + ac[2] + ac[1]) % 2;
				break;
			case 19:
				ac[0] = (ac[19] + ac[5] + ac[2] + ac[1]) % 2;
				break;
			case 20:
				ac[0] = (ac[20] + ac[3]) % 2;
				break;
			case 21:
				ac[0] = (ac[21] + ac[2]) % 2;
				break;
			case 22:
				ac[0] = (ac[22] + ac[1]) % 2;
				break;
			case 23:
				ac[0] = (ac[23] + ac[5]) % 2;
				break;
			case 24:
				ac[0] = (ac[24] + ac[4] + ac[3] + ac[1]) % 2;
				break;
			case 25:
				ac[0] = (ac[25] + ac[3]) % 2;
				break;
			case 26:
				ac[0] = (ac[26] + ac[6] + ac[2] + ac[1]) % 2;
				break;
			case 27:
				ac[0] = (ac[27] + ac[5] + ac[2] + ac[1]) % 2;
				break;
			case 28:
				ac[0] = (ac[28] + ac[3]) % 2;
				break;
			case 29:
				ac[0] = (ac[29] + ac[2]) % 2;
				break;
			case 30:
				ac[0] = (ac[30] + ac[6] + ac[4] + ac[2]) % 2;
				break;
			case 31:
				ac[0] = (ac[31] + ac[3]) % 2;
				break;
			case 32:
				ac[0] = (ac[32] + ac[7] + ac[5] + ac[3] + ac[2] + ac[1]) % 2;
				break;
			}
		}
	}

	if (mode == Random){

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 1);

		t_binary aux;
		for (int k = 0; k < process; k++) {
			aux = dis(gen);
			for (auto k = 0; k < numberOfOutputSignals; k++) {
				outputSignals[k]->bufferPut((t_binary)aux);
			}
			numberOfBits--;
		}
	}

	if (mode == DeterministicCyclic){
		std::vector<char> values(bitStream.begin(), bitStream.end());
		int valuesSize = (int) values.size();
		for (int k = 0; k < process; k++) {
			t_binary aux = (t_binary)(values[posBitStream++] - '0');
			for (auto k = 0; k < numberOfOutputSignals; k++) {
				outputSignals[k]->bufferPut((t_binary)aux);
			}
			numberOfBits--;
			posBitStream = posBitStream % valuesSize;
		}

	}

	if (mode == DeterministicAppendZeros){
		std::vector<char> values(bitStream.begin(), bitStream.end());
		int valuesSize = (int) values.size();
		t_binary aux;
		for (int k = 0; k < process; k++) {
			if (posBitStream == valuesSize) {
				aux = 0;
			}
			else {
				aux = (t_binary)(values[posBitStream++] - '0');
			}
			for (auto k = 0; k < numberOfOutputSignals; k++) {
				outputSignals[k]->bufferPut((t_binary)aux);
			}
			numberOfBits--;
		}

	}

	return true;
}

void BinarySource :: setBitPeriod(double bPeriod){
	bitPeriod = bPeriod;
	outputSignals[0]->setSymbolPeriod(bitPeriod);
	outputSignals[0]->setSamplingPeriod(outputSignals[0]->getSymbolPeriod());
};
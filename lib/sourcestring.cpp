#include <algorithm>
#include <complex>
#include <fstream>
#include <bitset>
#include <iostream>
#include <string>
#include "netxpto_20180418.h"
#include "sourcestring.h"

//string myString = "This is m";

int pos = 0;
string myString;

void SourceString::initialize(void) {
	
	char letter;
	int i;
	string line;

	ifstream reader("textIn.txt");

	if (reader) {
		for (i = 0; !reader.eof(); i++) {
			reader.get(letter);
			myString = myString + letter;
		}
		reader.close();
	}
	myString = myString.substr(0, myString.size() - 1);

	outputSignals[0]->setSymbolPeriod(1);
	outputSignals[0]->setSamplingPeriod(1);
}

bool SourceString::runBlock(void) {
	int space = outputSignals[0]->space();

	if (space == 0) return false;
	int out;

	while (space > 0) {

		if (pos == myString.size()) break;

		std::bitset<8> binVal(myString.c_str()[pos]);
		for (int k = 0; k < 8; k++) {
			outputSignals[0]->bufferPut((int)binVal[k]);
		}
		pos++;

		space = outputSignals[0]->space();
	}

	return false;
}

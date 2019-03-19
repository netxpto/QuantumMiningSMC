# ifndef BINARY_SOURCE_H_
# define BINARY_SOURCE_H_

# include <vector>
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
# include <chrono>
# include <bitset>

# include "netxpto_20180418.h"

enum BinarySourceMode { Random, PseudoRandom, DeterministicCyclic, DeterministicAppendZeros, AsciiFileAppendZeros, AsciiFileCyclic};

//using namespace std;

/* Generates a bit stream. Three types of sources are implemented (Random, PseudoRandom, DeterministicCyclic). In the Random mode the probability of generate a "0" is
going to be probabilityOfZero and probability of "1" is given by 1-probabilityOfZero. In the PseudoRandom mode, a PRBS sequence is generated with period
2^patternLength-1. In the DeterministicCyclic mode it is generated the sequence specified by bitStream.
If numberOfBits = -1 it generates an arbitrary large number of bits, otherwise the bit stream length equals numberOfBits.
The input parameter bitPerido specifies the bit period.
INPUT PARAMETERS:
BinarySourceMode type{ PseudoRandom };
double probabilityOfZero{ 0.5 };
int patternLength{ 7 };
string bitStream{ "01" };
long int numberOfBits{ -1 };
double bitPeriod{ 1.0 / 100e9 };
*/ 
class BinarySource : public Block {

public:

	//####################################### Constructors #########################################################

	BinarySource() {};
	BinarySource(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) : Block(InputSig, OutputSig) {};

	//################################# Public Member Functions ####################################################

	void setAsciiFileName(string nName) { asciiFilePath = nName; nextCharacterIndex = 0; nextBitIndex = 0; }
	string getAsciiFileName() const { return asciiFilePath; }

	void setMode(BinarySourceMode m) { mode = m; }
	BinarySourceMode getMode(void) const { return mode; };

	void setProbabilityOfZero(double pZero) { probabilityOfZero = pZero; };
	double getProbabilityOfZero(void) const { return probabilityOfZero; };

	void setBitStream(string bStream) { bitStream = bStream; };
	string getBitStream(void) const { return bitStream; };

	void setNumberOfBits(long int nOfBits) { numberOfBits = nOfBits; };
	long int getNumberOfBits(void) const { return numberOfBits; };

	void setPatternLength(int pLength) { patternLength = pLength; };
	int getPatternLength(void) const { return patternLength; };

	void setBitPeriod(double bPeriod) {	bitPeriod = bPeriod; outputSignals[0]->setSymbolPeriod(bitPeriod); outputSignals[0]->setSamplingPeriod(outputSignals[0]->getSymbolPeriod()); };
	double getBitPeriod(void) const { return bitPeriod; };


	//################################# Protected Member Functions ###################################################

	void initialize(void);

	bool runBlock(void);


private:

	// State variables
	std::vector<int> acumul;
	int posBitStream{ 0 };

	 // Input parameters
	 
	 BinarySourceMode mode{ PseudoRandom };
	 double probabilityOfZero{ 0.5 };
	 int patternLength{ 7 };
	 string bitStream{ "0100011101010101" };
	 string asciiFilePath{ "file_input_data.txt" };
	 
	 int nextCharacterIndex{ 0 };//Index of the next character
	 int nextBitIndex{ 0 }; //Index of the next bit to be read from the next character

	 long int numberOfBits{ -1 };
	 double bitPeriod{ 1.0 / 100e9 };

};

# endif



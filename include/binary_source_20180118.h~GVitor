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
#include <chrono>

# include "netxpto_20180118.h"

enum BinarySourceMode { Random, PseudoRandom, DeterministicCyclic, DeterministicAppendZeros };

using namespace std;

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

	// State variables
	std::vector<int> acumul;
	int posBitStream{ 0 };

 public:

	 // Input parameters
	 std::default_random_engine generator;
	 BinarySourceMode mode{ PseudoRandom };

	 double probabilityOfZero{ 0.5 };
	 int patternLength{ 7 };
	 string bitStream{ "0100011101010101" };

	 long int numberOfBits{ -1 };
	 double bitPeriod{ 1.0 / 100e9 };


	// Methods
	BinarySource() {};
	BinarySource(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};

	void initialize(void);
	
	bool runBlock(void);

	void setMode(BinarySourceMode m) {mode = m;}
	BinarySourceMode const getMode(void) { return mode; };
	
	void setProbabilityOfZero(double pZero) { probabilityOfZero = pZero; };
	double const getProbabilityOfZero(void) { return probabilityOfZero; };

	void setBitStream(string bStream) { bitStream = bStream; };
	string const getBitStream(void) { return bitStream; };

	void setNumberOfBits(long int nOfBits) { numberOfBits = nOfBits; };
	long int const getNumberOfBits(void) { return numberOfBits; };

	void setPatternLength(int pLength) { patternLength = pLength; };
	int const getPatternLength(void) { return patternLength; }
	
	void setBitPeriod(double bPeriod);
	double const getBitPeriod(void) { return bitPeriod; }

};

# endif



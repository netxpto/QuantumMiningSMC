#ifndef COINCIDENCE_DETECTOR_H
#define COINCIDENCE_DETECTOR_H

#include "netxpto_20180418.h"

class CoincidenceDetector : public Block {
	/*Input Parameters*/
	bool firstTime{ true };
public:

	CoincidenceDetector() {};
	CoincidenceDetector(vector<Signal *> &inputSignals, vector<Signal *> &outputSignals) : Block(inputSignals, outputSignals) {};
	//~CoincidenceDetector();

	void initialize(void);

	bool runBlock(void);

private:
	string fileName{"bits_statistic.txt"};
	/*State variables*/
	int bitsAlice{ 0 };
	int bitsAtten{ 0 };
	int bitsDoubl{ 0 };
};

#endif

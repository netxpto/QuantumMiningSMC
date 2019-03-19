#ifndef COINCIDENCE_DETECTOR_H
#define COINCIDENCE_DETECTOR_H

#include "../include/netxpto_20180815.h"

class CoincidenceDetector : public Block {
	
public:
	//############################################################################################################################

	CoincidenceDetector(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};
	CoincidenceDetector(vector<Signal *> &inputSignals, vector<Signal *> &outputSignals) : Block(inputSignals, outputSignals) {};
	//~CoincidenceDetector();

	//############################################################################################################################

	void initialize(void);
	bool runBlock(void);

	//############################################################################################################################
	void setDecisionLevel(double dLevel) { decisionLevel = dLevel; };
	double const getDecisionLevel() { return decisionLevel; };

private:
	/*Input Parameters*/
	double decisionLevel{ 0.5 };

	/*State variables*/
	bool on{ false };
	t_real value{ 3.0 };
};

#endif

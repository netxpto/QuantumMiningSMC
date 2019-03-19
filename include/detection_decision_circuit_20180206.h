#ifndef DETECTION_DECISION_CIRCUIT_H
#define DETECTION_DECISION_CIRCUIT_H

#include "netxpto_20180118.h"

class DetectionDecisionCircuit : public Block {
	/*Input Parameters*/
public:

	DetectionDecisionCircuit() {};
	DetectionDecisionCircuit(vector<Signal *> &inputSignals, vector<Signal *> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);

	bool runBlock(void);
};

#endif

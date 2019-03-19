# ifndef QNRG_DECISION_CIRCUIT_H_
# define QNRG_DECISION_CIRCUIT_H_

#include "netxpto_20180118.h"

class QRNG_DecisionCircuit : public Block {

	/* input parameters */

public:

	QRNG_DecisionCircuit(vector<Signal *> &inputSignals, vector<Signal *> &outputSignals) : Block(inputSignals, outputSignals){};

	void initialize(void);

	bool runBlock(void);

};

#endif
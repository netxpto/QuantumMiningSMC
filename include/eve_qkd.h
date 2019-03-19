#ifndef EVE_QKD_H_
#define EVE_QKD_H_

#include "netxpto_20180118.h"


class EveQKD : public Block {
	/* input parameters*/


public:
	EveQKD(vector <Signal*> &inputSignals, vector <Signal*> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);

	bool runBlock(void);

};

#endif

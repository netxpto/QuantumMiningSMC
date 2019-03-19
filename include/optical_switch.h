#ifndef OPTICAL_SWITCH_H_
#define OPTICAL_SWITCH_H_

#include "netxpto_20180118.h"

class OpticalSwitch : public Block {

public:
	OpticalSwitch(vector <Signal*> &inputSignals, vector <Signal*> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);

	bool runBlock(void);
};
#endif // !OPTICAL_SWITCH_H_

#ifndef ROTATOR_LINEAR_POLARIZER_H_
#define ROTATOR_LINEAR_POLARIZER_H_

#include "netxpto.h"

class RotatorLinearPolarizer : public Block {

public:
	RotatorLinearPolarizer(vector <Signal*> &inputSignals, vector <Signal*> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);

	bool runBlock(void);

	void setM(int mValue);

	void setAxis(vector <t_iqValues> AxisValues);
};
#endif // !ROTATOR_LINEAR_POLARIZER_H_
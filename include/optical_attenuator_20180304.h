#ifndef OPTICAL_ATTENUATOR_H
#define OPTICAL_ATTENUATOR_H

#include "netxpto_20180418.h"
#include <math.h>

class OpticalAttenuator : public Block {

private:
	/*Input parameters*/
	double kDB = 0.0; // in dB

public:
	/*Methods*/
	OpticalAttenuator() {};
	OpticalAttenuator(vector <Signal*> &inputSignals, vector <Signal*> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);
	bool runBlock(void);

	void setAttenuationCoef(double coef) { kDB = coef; };
	double getAttenuationCoef() { return kDB; }
};

#endif // !OPTICAL_ATTENUATOR_H

#pragma once
#pragma once
# ifndef PROGRAM_INCLUDE_MAGNITUDE_TO_PHASE_H_
# define PROGRAM_INCLUDE_MAGNITUDE_TO_PHASE_H_

# include "netxpto.h"
#include <random>

using namespace std;


class MagnitudeToPhase : public Block {

	bool firstTime{ true };

public:

	MagnitudeToPhase() {};
	MagnitudeToPhase(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setDcValue(double dcValue) { Eo = dcValue; };
	double const getDcValue(void) { return Eo; };

	void setIterations(int iter) { numberOfIterations = iter; };
	int const getIterations(void) { return numberOfIterations; };

private:

	double Eo{ 1 };
	int numberOfIterations{ 20 };
};

#endif // 
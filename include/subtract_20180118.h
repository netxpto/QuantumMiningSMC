# ifndef PROGRAM_INCLUDE_SUBTRACT_H_
# define PROGRAM_INCLUDE_SUBTRACT_H_

# include "netxpto_20180118.h"

// Simulates a simple subtract block 
class Subtract : public Block {

	bool firstTime{ true };

public:

	double outputOpticalPower{ 1e-3 };
	double outputOpticalWavelength{ 1550e-9 };
	double outputOpticalFrequency{ SPEED_OF_LIGHT / outputOpticalWavelength };

	Subtract() {};
	Subtract(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

private:
};


#endif // !PROGRAM_INCLUDE_SUBTRACT_H_

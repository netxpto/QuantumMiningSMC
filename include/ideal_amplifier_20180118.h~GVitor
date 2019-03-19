# ifndef PROGRAM_INCLUDE_IDEAL_AMPLIFIER_H_
# define PROGRAM_INCLUDE_IDEAL_AMPLIFIER_H_

# include "netxpto_20180118.h"
# include <vector>
#include <random>

// Simulates an ideal Amplifier
class IdealAmplifier : public Block {

	double gain{ 1e4 };

public:


	IdealAmplifier() {};
	IdealAmplifier(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);

	void setGain(double ga) { gain = ga; }
	double getGain() { return gain; }

};


#endif // !PROGRAM_INCLUDE_IDEAL_AMPLIFIER_H_

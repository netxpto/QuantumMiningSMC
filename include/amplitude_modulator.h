# ifndef PROGRAM_INCLUDE_AMPLITUDE_MODULATOR_H_
# define PROGRAM_INCLUDE_AMPLITUDE_MODULATOR_H_

# include "netxpto.h"

# include <complex>
# include <random>
# include <vector>


// this is a test block for the purpose of beta testing new code
// current code: Intensity Modulator.
class AmplitudeModulator : public Block {

	bool firstTime{ true };
	double dutyCycle = .25;
	int aux = 1;

public:

	AmplitudeModulator() {};
	AmplitudeModulator(vector<Signal*> &InputSig, vector<Signal*> &OutputSig) :Block(InputSig, OutputSig){};

	void initialize(void);
	bool runBlock(void);

	void setDutyCycle(double dCycle) { dutyCycle = dCycle; }
	double getDutyCycle() { return dutyCycle*100; }


};
#endif

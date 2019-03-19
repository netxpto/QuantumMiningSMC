# ifndef PROGRAM_INCLUDE_FIBER_H_
# define PROGRAM_INCLUDE_FIBER_H_

# include "netxpto.h"


// Implements an Optical Fiber
class Fiber : public Block {

	/* State Variables */

public:

	/* Input Parameters */

	t_real attenuationCoeficient{ 4.6052e-05 }; //m^(-1) correspond to 0.2 dB/km
	t_real dispersionCoeficient{ 0.0 }; //
	t_real fiberLength{ 0.0 }; //
	
	/* Methods */
	Fiber(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {}; 

	void initialize(void); //
	bool runBlock(void); //

	void setAttenuationCoeficient(t_real alpha) { attenuationCoeficient = alpha; } 
	t_real getAttenuationCoeficient() { return attenuationCoeficient; }

	void setDispersionCoeficient(t_real beta2) { dispersionCoeficient = beta2; }
	t_real getDispersionCoeficient() { return dispersionCoeficient; }

	void setFiberLength(t_real newFiberLength) { fiberLength = newFiberLength; }
	t_real getFiberLength(void) { return fiberLength; }
};

# endif // PROGRAM_INCLUDE_netxpto_H_


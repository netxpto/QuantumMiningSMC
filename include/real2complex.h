# ifndef PROGRAM_INCLUDE_REAL2COMPLEX_H_
# define PROGRAM_INCLUDE_REAL2COMPLEX_H_

# include <math.h>       /* pow */
# include "netxpto_20180118.h"


// Implements a IQ modulator.
class Real2Complex : public Block {

	/* State Variables */

	bool firstTime{ true };

 public:

	 /* Input Parameters */

	 double outputOpticalPower{ 1e-3 };
	 double outputOpticalWavelength{ 1550e-9 };
	 double outputOpticalFrequency{ SPEED_OF_LIGHT / outputOpticalWavelength };

	 /* Methods */
	 Real2Complex() {};
	 Real2Complex(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};

	 void initialize(void);
	 bool runBlock(void);

};

# endif // PROGRAM_INCLUDE_netxpto_H_



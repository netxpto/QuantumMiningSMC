# ifndef PROGRAM_INCLUDE_IQ_MODULATOR_H_
# define PROGRAM_INCLUDE_IQ_MODULATOR_H_

# include <math.h>       /* pow */
# include "netxpto_20180118.h"


// Implements a IQ modulator.
class IqModulator : public Block {

	/* State Variables */

	bool firstTime{ true };

 public:

	 /* Input Parameters */

	 double outputOpticalPower{ 1e-3 };
	 double outputOpticalWavelength{ 1550e-9 };
	 double outputOpticalFrequency{ SPEED_OF_LIGHT / outputOpticalWavelength };

	 /* Methods */
	 IqModulator() {};
	 IqModulator(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};

	 void initialize(void);
	 bool runBlock(void);

	 void setOutputOpticalPower(double outOpticalPower) { outputOpticalPower = outOpticalPower; }
	 void setOutputOpticalPower_dBm(double outOpticalPower_dBm) { outputOpticalPower = 1e-3*pow(10, outOpticalPower_dBm / 10); }

	 void setOutputOpticalWavelength(double outOpticalWavelength) { outputOpticalWavelength = outOpticalWavelength; outputOpticalFrequency = SPEED_OF_LIGHT / outOpticalWavelength; }
	 void setOutputOpticalFrequency(double outOpticalFrequency) { outputOpticalFrequency = outOpticalFrequency; outputOpticalWavelength = outOpticalFrequency / outputOpticalFrequency; }
};

# endif // PROGRAM_INCLUDE_netxpto_H_



# ifndef PROGRAM_INCLUDE_FREQUENCY_MISMATCH_COMPENSATION_H_
# define PROGRAM_INCLUDE_FREQUENCY_MISMATCH_COMPENSATION_H_


# include "netxpto_20180418.h"

# include <complex>
# include <random>
# include <vector>

class FrequencyMismatchCompensation : public Block {

	/* State Variables */


	/* Input Parameters */

	double pilotRate = 2;

public:

	/* Methods */
	FrequencyMismatchCompensation() {};
	FrequencyMismatchCompensation(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);

	void setPilotRate(double pRate) { pilotRate = pRate; }
	double getPilotRate() { return pilotRate; }


};

#endif 

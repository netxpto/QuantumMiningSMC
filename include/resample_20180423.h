# ifndef PROGRAM_INCLUDE_RESAMPLE_H_
# define PROGRAM_INCLUDE_RESAMPLE_H_


# include "netxpto_20180418.h"

# include <complex>
# include <random>
# include <vector>

class Resample : public Block {

	/* State Variables */


	/* Input Parameters */

	double rFactor{ 1.0 };
	double samplingPeriod{ 0.0 };
	double symbolPeriod{ 0.0 };
public:

	/* Methods */
	Resample() {};
	Resample(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);

	void setSamplingPeriod(double sPeriod) { samplingPeriod = sPeriod; }
	void setSymbolPeriod(double sPeriod) { symbolPeriod = sPeriod; }

	void setOutRateFactor(double OUTsRate) { rFactor = OUTsRate; }
	double getOutRateFactor() { return rFactor; }

};

#endif 

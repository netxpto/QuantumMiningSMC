# ifndef PROGRAM_INCLUDE_DOWNSAMPLING_H_
# define PROGRAM_INCLUDE_DOWNSAMPLING_H_


# include "netxpto_20180418.h"

# include <complex>
# include <random>
# include <vector>

class DownSampling : public Block {

public:

	/* Methods */
	//DownSampling() {};
	DownSampling(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);

	bool runBlock(void);

	//void setSamplingPeriod(double sPeriod) { samplingPeriod = sPeriod; }
	//double getSamplingPeriod() { return samplingPeriod; }

	void setSamplingFactor(unsigned int dSamplingfactor) { downSamplingFactor = dSamplingfactor; }
	unsigned int getSamplingFactor() { return downSamplingFactor; }


private: 

	/* State Variables */
	unsigned int samplesToSkip{ 0 };

	/* Input Parameters */

	unsigned int downSamplingFactor{1};
	//double samplingPeriod{ 0.0 };
	


};

#endif 

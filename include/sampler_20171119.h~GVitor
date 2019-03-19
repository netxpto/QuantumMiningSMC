# ifndef PROGRAM_INCLUDE_SAMPLER_H_
# define PROGRAM_INCLUDE_SAMPLER_H_

# include "netxpto_20180118.h"
# include <vector>

// Takes a sampling of the input electrical signal
class Sampler : public Block {

	bool firstTime{ true };


	bool firstPass{ true };
	bool repeatedPass{ false };
	int samplesToSkip{ 0 };
	int count = 0;
	bool aux1{ false };


public:

	double outputOpticalPower{ 1e-3 };

	void initialize(void);
	bool runBlock(void);

	void setSamplesToSkip(int sToSkip) { samplesToSkip = sToSkip; }
	int const getSamplesToSkip(void) { return samplesToSkip; }
	Sampler() {};
	Sampler(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	//void setSamplingRate(int Sampling) { sampling = Sampling; }
	//void setDelay(int Delay) { delay = Delay; }
	void setOutputOpticalPower(double outOpticalPower) { outputOpticalPower = outOpticalPower; }
	void setOutputOpticalPower_dBm(double outOpticalPower_dBm) { outputOpticalPower = 1e-3*pow(10, outOpticalPower_dBm / 10); }

};


#endif // !PROGRAM_INCLUDE_SAMPLER_H_

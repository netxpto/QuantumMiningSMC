# ifndef PROGRAM_INCLUDE_DC_SOURCE_H_
# define PROGRAM_INCLUDE_DC_SOURCE_H_


# include "netxpto.h"
# include <complex>
# include <random>
# include <vector>

class Dc : public Block {

	/* Input Parameters */

	double dcAmplitude{ 1.0 };
	double samplingPeriod{ 1.0 };
	double symbolPeriod{ 1.0 };


public:

	/* Methods */
	Dc() {};
	Dc(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) : Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setDcAmplitude(double dcAmpl) { dcAmplitude = dcAmpl; }
	double getDcAmplitude() { return dcAmplitude; }

	void setSamplingPeriod(double sPeriod) { samplingPeriod = sPeriod; }
	double getSamplingPeriod() { return samplingPeriod; }

	void setSymbolPeriod(double syPeriod) { symbolPeriod = syPeriod; }
	double getSymbolPeriod() { return symbolPeriod; }

};

#endif 

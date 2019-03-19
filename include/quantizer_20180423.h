# ifndef PROGRAM_INCLUDE_QUANTIZER_H_
# define PROGRAM_INCLUDE_QUANTIZER_H_


# include "netxpto_20180418.h"

# include <complex>
# include <random>
# include <vector>

class Quantizer : public Block {

	/* State Variables */


	/* Input Parameters */

	double nBits{ 64.0};
	double samplingPeriod{ 0.0 };
	double samplingRate{ 0.0 };
	double symbolPeriod{ 0.0 };
	double maxValue{ 1.5 };
	double minValue{ -1.5 };
public:

	/* Methods */
	Quantizer() {};
	Quantizer(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);

	void setSamplingPeriod(double sPeriod) { samplingPeriod = sPeriod; }
	void setSymbolPeriod(double sPeriod) { symbolPeriod = sPeriod; }

	void setResolution(double nbits) { nBits = nbits; }
	double getResolutions() { return nBits; }

	void setMinValue(double maxvalue) { maxValue = maxvalue; }
	double getMinValue() { return maxValue; }

	void setMaxValue(double maxvalue) { maxValue = maxvalue; }
	double getMaxValue() { return maxValue; }
};

#endif 

# ifndef PROGRAM_INCLUDE_ADC_H_
# define PROGRAM_INCLUDE_ADC_H_


# include "netxpto_20180118.h"

# include <complex>
# include <random>
# include <vector>

class ADC : public Block {

	/* State Variables */


	/* Input Parameters */

	double nBits{ 64.0};
	double samplingPeriod{ 0.0 };
	double ADCsamplingRate{ 0.0 };
	double symbolPeriod{ 0.0 };
	double maxValue{ 1.5 };
public:

	/* Methods */
	ADC() {};
	ADC(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);

	void setSamplingPeriod(double sPeriod) { samplingPeriod = sPeriod; }
	void setSymbolPeriod(double sPeriod) { symbolPeriod = sPeriod; }

	void setADCSamplingRate(double ADCsRate) { ADCsamplingRate = ADCsRate; }
	double getADCSamplingRate() { return ADCsamplingRate; }

	void setQuatizationBits(double nbits) { nBits = nbits; }
	double getQuatizationBits() { return nBits; }

	void setQuatizatiomaxValue(double maxvalue) { maxValue = maxvalue; }
	double getQuatizatiomaxValue() { return maxValue; }

};

#endif 

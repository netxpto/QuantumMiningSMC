# ifndef PROGRAM_INCLUDE_RF_OSCILLATOR_H_
# define PROGRAM_INCLUDE_RF_OSCILLATOR_H_


# include "netxpto.h"
# include <complex>
# include <random>
# include <vector>

class RfOscillator : public Block {

	/* Input Parameters */

	double rfAmplitude{ 1.0 };				
	double rfInitialPhase{ 0.0 };
	double rfFrequency{ 1 };
	double samplingPeriod{ 1.0 };
	double symbolPeriod{ 1.0 };
	double exponentialSign{ -1 };

	
	/* State Variables */
	double phase{ rfInitialPhase };

public:

	/* Methods */
	RfOscillator() {};
	RfOscillator(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) : Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setSamplingPeriod(double sPeriod) { samplingPeriod = sPeriod; }					
	double getSamplingPeriod() { return samplingPeriod; }

	void setSymbolPeriod(double syPeriod) { symbolPeriod = syPeriod; }					
	double getSymbolPeriod() { return symbolPeriod; }

	void setrfAmplitude(double rfAmpl) { rfAmplitude = rfAmpl; }							
	double getRfAmplitude() { return rfAmplitude; }

	void setrfFrequency(double rfFreq) { rfFrequency = rfFreq; }							
	double getrfFrequency() { return rfFrequency; }

	void setrfPhase(double rfPh) { rfInitialPhase = rfPh; }									
	double getRfPhase() { return rfInitialPhase; }

	void setExponentialSign(double sign) { exponentialSign = sign; }
	double getSign() { return exponentialSign; }
	
};

#endif 

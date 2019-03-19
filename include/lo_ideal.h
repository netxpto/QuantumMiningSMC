# ifndef PROGRAM_INCLUDE_LO_IDEAL_H_
# define PROGRAM_INCLUDE_LO_IDEAL_H_


# include "netxpto.h"

# include <complex>
# include <random>
# include <vector>

class LocalOscillator : public Block {

	/* State Variables */


	/* Input Parameters */

	double opticalPower{ 1e-3 };
	double outputOpticalWavelength{ 1550e-9 };
	double outputOpticalFrequency{ SPEED_OF_LIGHT / outputOpticalWavelength };
	double phase{ 0 };
	double samplingPeriod{ 0.0 };
	double symbolPeriod{ 0.0 };

	default_random_engine generatorRIN;
	double signaltoNoiseRatio{ 0 };

public:

	/* Methods */
	LocalOscillator() {};
	LocalOscillator(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setSamplingPeriod(double sPeriod) { samplingPeriod = sPeriod; }
	void setSymbolPeriod(double sPeriod) { symbolPeriod = sPeriod; }
	void setOpticalPower(double oPower) { opticalPower = oPower; }
	void setOpticalPower_dBm(double oPower_dBm) { opticalPower = 1e-3*pow(10, oPower_dBm / 10); }

	void setWavelength(double wlength) { outputOpticalWavelength = wlength;  outputOpticalFrequency = SPEED_OF_LIGHT / outputOpticalWavelength; }
	double getWavelength() { return outputOpticalWavelength; }

	void setFrequency(double freq) { outputOpticalFrequency = freq;  outputOpticalWavelength = SPEED_OF_LIGHT / outputOpticalFrequency; }
	double getFrequency() { return outputOpticalFrequency; }

	void setPhase(double lOscillatorPhase) { phase = lOscillatorPhase; }
	double const getPhase(void) { return phase; }

	void setSignaltoNoiseRatio(double sNoiseRatio) { signaltoNoiseRatio = sNoiseRatio; }
	double const getSignaltoNoiseRatio(void) { return signaltoNoiseRatio; }

};

#endif 

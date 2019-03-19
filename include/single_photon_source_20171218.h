# ifndef SINGLE_PHOTON_SOURCE_H_
# define SINGLE_PHOTON_SOURCE_H_

# include <algorithm>  // min

#include "netxpto_20180418.h"

class SinglePhotonSource : public Block {

	

public:
	/* input parameters */
	double outputOpticalWavelength{ 1550e-9 };
	double outputOpticalPower{ 1e-3 };
	double outputOpticalFrequency{ SPEED_OF_LIGHT / outputOpticalWavelength };
	double SymbolPeriod{ 0.0 };
	double samplingPeriod{ 0.0 };


	/*	Methods	*/

	SinglePhotonSource(vector<Signal *> &inputSignals, vector<Signal *> &outputSignals) : Block(inputSignals, outputSignals){};

	void initialize(void);
	bool runBlock(void);

	void setOutputOpticalPower(double outOpticalPower) { outputOpticalPower = outOpticalPower; }
	void setOutputOpticalPower_dBm(double outOpticalPower_dBm) { outputOpticalPower = 1e-3*pow(10, outOpticalPower_dBm / 10); }

	void setOuputOpticalWavelength(double outOpticalWavelength) { outputOpticalWavelength = outOpticalWavelength; outputOpticalFrequency = SPEED_OF_LIGHT / outOpticalWavelength; }
	void setOutputOpticalFrequency(double outOpticalFrequency) { outputOpticalFrequency = outOpticalFrequency; outputOpticalWavelength = outOpticalFrequency / outputOpticalFrequency; }

	void setSamplingPeriod(double per) { samplingPeriod = per; }
	double getSamplingPeriod() { return samplingPeriod; }

	void setSymbolPeriod(double symbper) { SymbolPeriod = symbper; }
	double getSymbolPeriod() { return SymbolPeriod; }


};

#endif

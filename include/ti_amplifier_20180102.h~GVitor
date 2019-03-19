# ifndef PROGRAM_INCLUDE_TIAMPLIFIER_H_
# define PROGRAM_INCLUDE_TIAMPLIFIER_H_

# include "netxpto_20180118.h"

# include "add_20180118.h"
# include "ideal_amplifier_20180118.h"
# include "white_noise_20180118.h"
# include "pulse_shaper_20180118.h"

// Trans-Impedance Amplifier superblock
class TI_Amplifier : public SuperBlock {

	// #####################################################################################################
	// ################## Internal Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	TimeContinuousAmplitudeContinuousReal TI_AMP01{ "TI_AMP01.sgn" }; // Amplified

	TimeContinuousAmplitudeContinuousReal TI_AMP02{ "TI_AMP02.sgn" }; // Noise

	TimeContinuousAmplitudeContinuousReal TI_AMP03{ "TI_AMP03.sgn" }; // Noisy Amplified Signal

	TimeContinuousAmplitudeContinuousReal TI_AMP04{ "TI_AMP04.sgn" }; // Filtered Noisy Signal

	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################

	IdealAmplifier B1;

	WhiteNoise B2;
	
	Add B3;

	PulseShaper B4;

	/* State Variables */

	/* Input Parameters */


	bool bypassFilter{ true };

	bool firsTime{ true };

public:

	/* Methods */

	
	TI_Amplifier(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal);
	TI_Amplifier() {};

	/* Set Methods */

	void setGain(double ga) { B1.setGain(ga); };
	double const getGain(void) { B1.getGain(); }

	void setElectricalNoiseSpectralDensity(double eNoiseSpectralDensity) { B2.setNoiseSpectralDensity(eNoiseSpectralDensity); }
	double const getElectricalNoiseSpectralDensity(void) { B2.getNoiseSpectralDensity(); }

	void setRollOffFactor(double rOffFactor) { B4.setRollOffFactor(rOffFactor); };
	double const getRollOffFactor(void) { return B4.getRollOffFactor(); };
	
	void setImpulseResponseTimeLength(int impResponseTimeLength) { B4.setImpulseResponseTimeLength(impResponseTimeLength); };
	int const getImpulseResponseTimeLength(void) { return B4.getImpulseResponseTimeLength(); };

	void setImpulseResponseLength(int impResponseLength) { B4.setImpulseResponseLength(impResponseLength); };
	int const getImpulseResponseLength(void) { return B4.getImpulseResponseLength(); };

	void setBypassFilter(bool bFilter){ bypassFilter = bFilter; };

	void usePassiveFilterMode(bool pFilterMode){ B4.usePassiveFilterMode(pFilterMode); };

	void setSeeBeginningOfImpulseResponse(bool sBegginingOfImpulseResponse) { B4.setSeeBeginningOfImpulseResponse(sBegginingOfImpulseResponse); };
	double const getSeeBeginningOfImpulseResponse(void) { return B4.getSeeBeginningOfImpulseResponse(); };

};


#endif

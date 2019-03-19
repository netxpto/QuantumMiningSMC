# ifndef PROGRAM_INCLUDE_I_HOMODYNE_RECEIVER_H_
# define PROGRAM_INCLUDE_I_HOMODYNE_RECEIVER_H_

# include "netxpto_20180118.h"

# include "photodiode_old_20180118.h"
# include "ideal_amplifier_20180118.h"
# include "white_noise_20180118.h"
# include "add_20171116.h"

// Balanced Homodyne Receiver superblock
class I_HomodyneReceiver : public SuperBlock {

	// #####################################################################################################
	// ################## Internal Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	TimeContinuousAmplitudeContinuousReal I_HMD01{ "I_HMD01.sgn" }; // Detected and Subtracted -> 4*S*LO

	TimeContinuousAmplitudeContinuousReal I_HMD02{ "I_HMD02.sgn" }; // Amplified
	
	TimeContinuousAmplitudeContinuousReal I_HMD03{ "I_HMD03.sgn" }; // Noise  Noisy and Filtered

	TimeContinuousAmplitudeContinuousReal I_HMD04{ "I_HMD04.sgn" }; // Added  

	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################

	Photodiode B1;

	IdealAmplifier B2;

	WhiteNoise B3;

	Add B4;

	/* State Variables */

	/* Input Parameters */
	double samplingPeriod{ 0.0 };

	double localOscillatorOpticalPower{ 1e-3 };
	double localOscillatorPhase{ 0 };
	double localOscillatorWavelength{ 1550e-9 };

	int samplesToSkip{ 0 };

	double outputOpticalFrequency{ SPEED_OF_LIGHT / localOscillatorWavelength };

public:

	/* Methods */

	I_HomodyneReceiver() {};
	I_HomodyneReceiver(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal);

	/* Set Methods */

	void setResponsivity(double Responsivity) { B1.setResponsivity(Responsivity); };
	double const getResponsivity(void) { B1.getResponsivity(); }
	void useShotNoise(bool uShotNoise){ B1.useNoise(uShotNoise); };

	void setGain(double Gain) { B2.setGain(Gain); }
    double const getGain(void) { return B2.getGain(); };
    
	void setElectricalNoiseSpectralDensity(double eNoiseSpectralDensity) { B3.setNoiseSpectralDensity(eNoiseSpectralDensity); }
	double const getElectricalNoiseSpectralDensity(void) { return B3.getNoiseSpectralDensity(); }
    
    
/*    void setRollOffFactor(double rOffFactor) {B2.setRollOffFactor(rOffFactor); }
	double const getRollOffFactor(void) { return B2.getRollOffFactor(); };
    
    void setImpulseResponseTimeLength(int impResponseTimeLength) {B2.setImpulseResponseTimeLength(impResponseTimeLength); };
    int const getImpulseResponseTimeLength(void) {return B2.getImpulseResponseTimeLength(); };
    
    void setImpulseResponseLength(int impResponseLength) {B2.setImpulseResponseLength(impResponseLength); }
    int const getImpulseResponseLength(void) { return B2.getImpulseResponseLength();};
    
	void usePassiveFilterMode(bool pFilterMode){ B2.usePassiveFilterMode(pFilterMode); }*/
};

#endif

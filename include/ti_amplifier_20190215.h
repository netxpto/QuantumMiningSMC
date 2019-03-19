# ifndef PROGRAM_INCLUDE_TIAMPLIFIER_H_
# define PROGRAM_INCLUDE_TIAMPLIFIER_H_

# include "netxpto_20190215.h"

# include "add_20190215.h"
# include "ideal_amplifier_20190215.h"
# include "white_noise_20190215.h"
# include "electrical_filter_20190215.h" 

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

	WhiteNoise B1;
	Add B2;
	IdealAmplifier B3;
	ElectricalFilter B4;

	/* State Variables */

	/* Input Parameters */

	bool firsTime{ true };

public:

	/* Methods */

	TI_Amplifier() {};
	TI_Amplifier(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal);


	/* Set Methods */

	// Signal gain is assumed to be equal to internal noise gain.
	void setGain(double ga) { B3.setGain(ga); };
	double const getGain(void) { B3.getGain(); }

	void setInputReferredNoisePowerSpectralDensity(double newInputReferredNPDS) { B1.setNoiseSpectralDensity(newInputReferredNPDS); }
	double const getInputReferredNoisePowerSpectralDensity(void) { B1.getNoiseSpectralDensity(); }

	void setNoiseSamplingPeriod(t_real samplingPeriod) { B1.setSamplingPeriod(samplingPeriod); }
	t_real getNoiseSamplingPeriod(void) { return B1.getSamplingPeriod(); };

	void setFilterType(Filter filterType) { B4.setFilterType(filterType); };
	double const getFilterType(void) { return B4.getFilterType(); };
	
	void setCutoffFrequency(double cutoffFreq) { B4.setCutoffFrequency(cutoffFreq); };
	double const getCutoffFrequency(void) { return B4.getCutoffFrequency(); };

	void setImpulseResponseTimeLength(int impResponseTimeLength) { B4.setImpulseResponseTimeLength(impResponseTimeLength); };
	int const getImpulseResponseTimeLength(void) { return B4.getImpulseResponseTimeLength(); };

	void setImpulseResponse(vector<t_real> ir) { B4.setImpulseResponse(ir); };
	vector<t_real> getImpulseResponse(void) { B4.getImpulseResponse(); };

	void setImpulseResponseFilename(string fName) { B4.setImpulseResponseFilename(fName); }
	string getImpulseResponseFilename(void) { B4.getImpulseResponseFilename(); }

	void setImpulseResponseLength(int impResponseLength) { B4.setImpulseResponseLength(impResponseLength); };
	int const getImpulseResponseLength(void) { return B4.getImpulseResponseLength(); };

	void setSeeBeginningOfImpulseResponse(bool sBegginingOfImpulseResponse) { B4.setSeeBeginningOfImpulseResponse(sBegginingOfImpulseResponse); };
	double const getSeeBeginningOfImpulseResponse(void) { return B4.getSeeBeginningOfImpulseResponse(); };
	
	void setDirName(string newDirName);
	string getDirName(void) { return dirName; }


private:
	string dirName{ "" };
};


#endif

# ifndef PROGRAM_INCLUDE_TIAMPLIFIER_H_
# define PROGRAM_INCLUDE_TIAMPLIFIER_H_

# include "netxpto_20180118.h"

# include "add_20171116.h"
# include "ideal_amplifier_20180118.h"
# include "white_noise_20180420.h"
# include "electrical_filter_20180625.h" 

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
	ElectricalFilter B4;

	/* State Variables */

	/* Input Parameters */


	bool bypassFilter{ true };

	bool firsTime{ true };

public:

	/* Methods */

	TI_Amplifier() {};
	TI_Amplifier(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal);


	/* Set Methods */

	void setGain(double ga) { B1.setGain(ga); };
	double const getGain(void) { B1.getGain(); }

	void setElectricalNoiseSpectralDensity(double eNoiseSpectralDensity) { B2.setNoiseSpectralDensity(eNoiseSpectralDensity); }
	double const getElectricalNoiseSpectralDensity(void) { B2.getNoiseSpectralDensity(); }

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

};


#endif

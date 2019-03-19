# ifndef PROGRAM_INCLUDE_HOMODYNE_RECEIVER_withoutLO_H_
# define PROGRAM_INCLUDE_HOMODYNE_RECEIVER_withoutLO_H_

# include "netxpto_20180118.h"

# include "optical_hybrid_20180118.h"
# include "photodiode_old_20180118.h"
# include "ideal_amplifier_20180118.h"
# include "decoder_20180118.h"
# include "sampler_20171119.h"
# include "super_block_interface_20180118.h"
# include "pulse_shaper_20180118.h"
# include "white_noise_20180420.h"
# include "add_20171116.h"
# include "snr_estimator_20180227.h"
# include "electrical_filter_20180625.h"
# include "power_spectral_density_estimator_20180704.h"
# include "ti_amplifier_20180214.h"

class HomodyneReceiverWithoutLO : public SuperBlock {

	// #####################################################################################################
	// ################## Internal Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	OpticalSignal HMD01{ "HMD01.sgn" }; // S+L
	OpticalSignal HMD02{ "HMD02.sgn" }; // S-L
	OpticalSignal HMD03{ "HMD03.sgn" }; // S+iL
	OpticalSignal HMD04{ "HMD04.sgn" }; // S-ilL

	TimeContinuousAmplitudeContinuousReal HMD05{ "HMD05.sgn" }; // Photodiode's output
	TimeContinuousAmplitudeContinuousReal HMD06{ "HMD06.sgn" }; // Photodiode's output

	TimeContinuousAmplitudeContinuousReal HMD07{ "HMD07.sgn" }; // TI_Amplifier output
	TimeContinuousAmplitudeContinuousReal HMD08{ "HMD08.sgn" }; // TI_Amplifier output

	TimeContinuousAmplitudeContinuousReal HMD09{ "HMD09.sgn" }; // White noise
	TimeContinuousAmplitudeContinuousReal HMD10{ "HMD10.sgn" }; // White noise

	TimeContinuousAmplitudeContinuousReal HMD11{ "HMD11.sgn" }; // Add
	TimeContinuousAmplitudeContinuousReal HMD12{ "HMD12.sgn" }; // Add

	TimeContinuousAmplitudeContinuousReal HMD13{ "HMD13.sgn" }; // Filtered
	TimeContinuousAmplitudeContinuousReal HMD14{ "HMD14.sgn" }; // Filtered

	TimeContinuousAmplitudeContinuousReal HMD15{ "HMD15.sgn" }; // Power Spectral Density Block Output
	TimeContinuousAmplitudeContinuousReal HMD16{ "HMD16.sgn" }; // Power Spectral Density Block Output

	TimeContinuousAmplitudeContinuousReal HMD17{ "HMD17.sgn" }; // SNREstimator block output
	TimeContinuousAmplitudeContinuousReal HMD18{ "HMD18.sgn" }; // SNREstimator block output

	TimeDiscreteAmplitudeContinuousReal HMD19{ "HMD19.sgn" }; // Sampled 
	TimeDiscreteAmplitudeContinuousReal HMD20{ "HMD20.sgn" }; // Sampled 

	Binary HMD21{ "HMD21.sgn" }; // recovery Sequence


	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################

	OpticalHybrid B01;

	Photodiode B02;
	Photodiode B03;
	
	TI_Amplifier B04{ vector<Signal*> { &HMD05 }, vector<Signal*> { &HMD07 } };
	TI_Amplifier B05{ vector<Signal*> { &HMD06 }, vector<Signal*> { &HMD08 } };

	WhiteNoise B06;
	WhiteNoise B07;

	Add B08;
	Add B09;
	
	PulseShaper B10; // Matched Filter
	PulseShaper B11; // Matched Filter

	PowerSpectralDensityEstimator B12;
	SNREstimator B13;

	Sampler B14;
	Sampler B15;
	
	Decoder B16;

	/* State Variables */

	/* Input Parameters */
	double samplingPeriod{ 0.0 };

public:

	/* Methods */

	HomodyneReceiverWithoutLO(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal);

	/* Set Methods */

	// Decoder config
	void setIqAmplitudes(vector<t_iqValues> iqAmplitudesValues) { B16.setIqAmplitudes(iqAmplitudesValues); };
	vector<t_iqValues> const getIqAmplitudes(void) { return B16.getIqAmplitudes(); };

	// Photodiodes config
	void  setResponsivity(t_real Responsivity) { B02.setResponsivity(Responsivity); B03.setResponsivity(Responsivity); };

	/*
	// Amplifier config
	void setAmplification(t_real Amplification) { B04.setGain(Amplification); B05.setGain(Amplification); };
	*/

	// TI Amplifier config
	void setGain(t_real gain) { B04.setGain(gain); B05.setGain(gain); };
	t_real getGain(void) { B04.getGain(); };
	void setAmplifierNoiseSpectralDensity(t_real NoiseSpectralDensity) { B04.setElectricalNoiseSpectralDensity(NoiseSpectralDensity); B05.setElectricalNoiseSpectralDensity(NoiseSpectralDensity); };
	t_real getAmplifierNoiseSpectralDensity(void) { B04.getElectricalNoiseSpectralDensity(); };
	void setElFilterType(Filter fType) { B04.setFilterType(fType); B05.setFilterType(fType); };
	void setCutoffFrequency(double ctfFreq) { B04.setCutoffFrequency(ctfFreq); B05.setCutoffFrequency(ctfFreq); }
	void setElFilterImpulseResponseTimeLength(int irl) { B04.setImpulseResponseTimeLength(irl);  B05.setImpulseResponseTimeLength(irl); }
	void setElFilterImpulseResponse(vector<t_real> ir) { B04.setImpulseResponse(ir); B05.setImpulseResponse(ir);; }
	void setEFImpulseResponseFilename(string fName) { B04.setImpulseResponseFilename(fName); B05.setImpulseResponseFilename(fName); }
	void setEFSeeBeginningOfImpulseResponse(bool sBeginningOfImpulseResponse) { B05.setSeeBeginningOfImpulseResponse(sBeginningOfImpulseResponse); B04.setSeeBeginningOfImpulseResponse(sBeginningOfImpulseResponse); };
	double const getEFSeeBeginningOfImpulseResponse(void) { return B05.getSeeBeginningOfImpulseResponse(); };
	void setEFFirstFilteredValueToBeSaved(int value) { HMD07.setFirstValueToBeSaved(value); HMD08.setFirstValueToBeSaved(value); }; // SeeBeginningOfImpulseResponse does not remove
	int getEFFirstFilteredValueToBeSaved() { HMD07.getFirstValueToBeSaved(); };													 // all unwanted values from the signal.
	//	void setFirstFilteredValueToBeSavedEl(int value) { HMD07.setFirstValueToBeSaved(value); HMD08.setFirstValueToBeSaved(value);}; // SeeBeginningOfImpulseResponse does not remove
//	int getFirstFilteredValueToBeSavedEl() { HMD07.getFirstValueToBeSaved(); };													 // all unwanted values from the signal.


	// Thermal noise config
	void setThermalNoiseSpectralDensity(t_real NoiseSpectralDensity) { B06.setNoiseSpectralDensity(NoiseSpectralDensity); B07.setNoiseSpectralDensity(NoiseSpectralDensity);};
	void setNoiseSamplingPeriod(t_real SamplingPeriod) { B06.setSamplingPeriod(SamplingPeriod); B07.setSamplingPeriod(SamplingPeriod); };
	void setSeeds(array<int,2> noiseSeeds) { B06.setSeed(noiseSeeds[0]); B07.setSeed(noiseSeeds[1]); };
	void setSeedType(SeedType seedType) { B06.setSeedType(seedType); B07.setSeedType(seedType); };

	// Pulse shaper config
	void setImpulseResponseTimeLength(int impResponseTimeLength) { B10.setImpulseResponseTimeLength(impResponseTimeLength); B11.setImpulseResponseTimeLength(impResponseTimeLength); };
	void setFilterType(PulseShaperFilter fType) { B10.setFilterType(fType); B11.setFilterType(fType); };
	void setRollOffFactor(double rOffFactor) { B10.setRollOffFactor(rOffFactor); B11.setRollOffFactor(rOffFactor); };
	void usePassiveFilterMode(bool pFilterMode) { B10.usePassiveFilterMode(pFilterMode); B11.usePassiveFilterMode(pFilterMode); }
	void setMFImpulseResponseFilename(string fName) { B10.setImpulseResponseFilename(fName); B11.setImpulseResponseFilename(fName); }
	void setFirstFilteredValueToBeSaved(int value) { HMD15.setFirstValueToBeSaved(value); HMD16.setFirstValueToBeSaved(value);}; // SeeBeginningOfImpulseResponse does not remove
	int getFirstFilteredValueToBeSaved() { HMD15.getFirstValueToBeSaved(); };													 // all unwanted values from the signal.
	void setMFSeeBeginningOfImpulseResponse(bool sBeginningOfImpulseResponse) { B10.setSeeBeginningOfImpulseResponse(sBeginningOfImpulseResponse); B11.setSeeBeginningOfImpulseResponse(sBeginningOfImpulseResponse); };
	double const getMFSeeBeginningOfImpulseResponse(void) { return B10.getSeeBeginningOfImpulseResponse(); };

	// Sampler config
	void setSamplesToSkip(int sToSkip) { B14.setSamplesToSkip(sToSkip); B15.setSamplesToSkip(sToSkip); };
	void setSamplerOpticalPower_dBm(double optPower_dBm) { B14.setOutputOpticalPower_dBm(optPower_dBm); B15.setOutputOpticalPower_dBm(optPower_dBm); };
	
	// SNR estimators config
	void setFilteredSNRFilename(string fname) { B13.setFilename(fname); }
	void setSNRRollOffComp(double rollOff) { B13.setRollOff(rollOff);}
	void setSNRnoiseBw(double nBw) { B13.setNoiseBw(nBw);}
	void setSNR2method(SNREstimatorMethod mtd) { B13.setEstimatorMethod(mtd); }
	void setIgnoreInitialSamples2(int iis) { B13.setIgnoreInitialSamples(iis); }
	void setSegmentSize(int sSz) { B13.setSegmentSize(sSz); }
	void setOverlapPercent(double olp) { B13.setOverlapPercent(olp);}

	// Power Spectral Density Config
	void setPSDFilename(string fname) { B12.setFilename(fname); }
	string getPSDFilename(void) { return B12.getFilename(); }
	void setPSDMeasuredIntervalSize(int msz) { B12.setMeasuredIntervalSize(msz); }
	int getPSDMeasuredIntervalSize(void) { B12.getMeasuredIntervalSize(); }
	void setPSDSegmentSize(int msz) { B12.setSegmentSize(msz); }
	int getPSDSegmentSize(void) { B12.getSegmentSize(); }
	void setPSDOverlapPercent(double olp) { B12.setOverlapPercent(olp); }
	double getPSDOverlapPercent(void) { B12.getOverlapPercent(); }
	void setPSDConfInterval(double P) { B12.setConfidence(P); }
	double getPSDConfInterval(void) { B12.getConfidence(); }

	void setDirName(string newDirName);
	string getDirName(void) { return dirName; }

	private:
		string dirName{ "" };
};

#endif

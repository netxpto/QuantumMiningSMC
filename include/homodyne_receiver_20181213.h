# ifndef PROGRAM_INCLUDE_HOMODYNE_RECEIVER_H_
# define PROGRAM_INCLUDE_HOMODYNE_RECEIVER_H_

# include "netxpto_20180118.h"

# include "optical_hybrid_20180118.h"
# include "photodiode_old_20180118.h"
# include "ideal_amplifier_20180118.h"
# include "decoder_20180118.h"
# include "local_oscillator_20180118.h"
# include "sampler_20171119.h"
# include "super_block_interface_20180118.h"
# include "pulse_shaper_20180118.h"
# include "white_noise_20180420.h"
# include "add_20171116.h"
# include "snr_estimator_20180227.h"
# include "electrical_filter_20180625.h"
# include "power_spectral_density_estimator_20180704.h"

// this is a test block for the purpose of beta testing new code
// current code: Building Homodyne superblock.
class HomodyneReceiver : public SuperBlock {

	// #####################################################################################################
	// ################## Internal Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	OpticalSignal HMD01{ "HMD01.sgn" }; // Local Oscillator

	OpticalSignal HMD02{ "HMD02.sgn" }; // S+L
	OpticalSignal HMD03{ "HMD03.sgn" }; // S-L
	OpticalSignal HMD04{ "HMD04.sgn" }; // S+iL
	OpticalSignal HMD05{ "HMD05.sgn" }; // S-ilL

	TimeContinuousAmplitudeContinuousReal HMD06{ "HMD06.sgn" }; // Photodiode's output
	TimeContinuousAmplitudeContinuousReal HMD07{ "HMD07.sgn" }; // Photodiode's output

	TimeContinuousAmplitudeContinuousReal HMD08{ "HMD08.sgn" }; // Ideal Amplifier output
	TimeContinuousAmplitudeContinuousReal HMD09{ "HMD09.sgn" }; // Ideal Amplifier output

	TimeContinuousAmplitudeContinuousReal HMD10{ "HMD10.sgn" }; // White noise
	TimeContinuousAmplitudeContinuousReal HMD11{ "HMD11.sgn" }; // White noise

	TimeContinuousAmplitudeContinuousReal HMD12{ "HMD12.sgn" }; // Add
	TimeContinuousAmplitudeContinuousReal HMD13{ "HMD13.sgn" }; // Add

	TimeContinuousAmplitudeContinuousReal HMD14{ "HMD14.sgn" }; // Electrical Filter Output
	TimeContinuousAmplitudeContinuousReal HMD15{ "HMD15.sgn" }; // Electrical Filter Output

	TimeContinuousAmplitudeContinuousReal HMD16{ "HMD16.sgn" }; // White noise
	TimeContinuousAmplitudeContinuousReal HMD17{ "HMD17.sgn" }; // White noise

	TimeContinuousAmplitudeContinuousReal HMD18{ "HMD18.sgn" }; // Add
	TimeContinuousAmplitudeContinuousReal HMD19{ "HMD19.sgn" }; // Add

	TimeContinuousAmplitudeContinuousReal HMD20{ "HMD20.sgn" }; // Power Spectral Density block output
	TimeContinuousAmplitudeContinuousReal HMD21{ "HMD21.sgn" }; // Power Spectral Density block output

	TimeContinuousAmplitudeContinuousReal HMD22{ "HMD22.sgn" }; // SNREstimator block output
	TimeContinuousAmplitudeContinuousReal HMD23{ "HMD23.sgn" }; // SNREstimator block output

	TimeContinuousAmplitudeContinuousReal HMD24{ "HMD24.sgn" }; //Filtered (pulse shaper)
	TimeContinuousAmplitudeContinuousReal HMD25{ "HMD25.sgn" }; //Filtered (pulse shaper)

	TimeContinuousAmplitudeContinuousReal HMD26{ "HMD26.sgn" }; // SNREstimator block output
	TimeContinuousAmplitudeContinuousReal HMD27{ "HMD27.sgn" }; // SNREstimator block output

	TimeDiscreteAmplitudeContinuousReal HMD28{ "HMD28.sgn" }; // Sampled 
	TimeDiscreteAmplitudeContinuousReal HMD29{ "HMD29.sgn" }; // Sampled 

	Binary HMD30{ "HMD30.sgn" }; // recovery Sequence


	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################

	LocalOscillator B1;

	OpticalHybrid B2;

	Photodiode B3;
	Photodiode B4;

	IdealAmplifier B5;
	IdealAmplifier B6;

	WhiteNoise B7;
	WhiteNoise B8;

	Add B9;
	Add B10;

	ElectricalFilter B11;
	ElectricalFilter B12;

	WhiteNoise B13;
	WhiteNoise B14;

	Add B15;
	Add B16;

	PowerSpectralDensityEstimator B17;

	SNREstimator B18;

	PulseShaper B19; // Matched Filter
	PulseShaper B20; // Matched Filter

	SNREstimator B21;

	Sampler B22;
	Sampler B23;
	
	Decoder B24;

	/* State Variables */

	/* Input Parameters */
	double samplingPeriod{ 0.0 };

	double localOscillatorOpticalPower{ 1e-3 };
	double localOscillatorPhase{ 0 };
	double localOscillatorWavelength{ 1550e-9 };

	//int samplesToSkip{ 0 };

	double outputOpticalFrequency{ SPEED_OF_LIGHT / localOscillatorWavelength };

public:

	/* Methods */

	HomodyneReceiver(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal);

	/* Set Methods */

	// Decoder config
	void setIqAmplitudes(vector<t_iqValues> iqAmplitudesValues) { B24.setIqAmplitudes(iqAmplitudesValues); };
	vector<t_iqValues> const getIqAmplitudes(void) { return B24.getIqAmplitudes(); };

	// Local Oscillator config
	void setLocalOscillatorSamplingPeriod(double sPeriod) { B1.setSamplingPeriod(sPeriod); };
	void setLocalOscillatorOpticalPower(double opticalPower) { B1.setOpticalPower(opticalPower); };
	void setLocalOscillatorOpticalPower_dBm(double opticalPower_dBm) { B1.setOpticalPower_dBm(opticalPower_dBm); };
	void setLocalOscillatorPhase(double lOscillatorPhase) { B1.setPhase(lOscillatorPhase); };
	void setLocalOscillatorOpticalWavelength(double lOscillatorWavelength) { B1.setWavelength(lOscillatorWavelength); };
	void setSamplingPeriod(double sPeriod) { B1.setSamplingPeriod(sPeriod); };//B13A.setSamplingPeriod(sPeriod); B13B.setSamplingPeriod(sPeriod);
	//};

	// Amplifier config
	void  setResponsivity(t_real Responsivity) { B3.setResponsivity(Responsivity); B4.setResponsivity(Responsivity); };
	void setAmplification(t_real Amplification) { B5.setGain(Amplification); B6.setGain(Amplification); };
	
	// Noise sources config
	void setPreFilterNoiseSpectralDensity(t_real NoiseSpectralDensity) { B7.setNoiseSpectralDensity(NoiseSpectralDensity / 2); B8.setNoiseSpectralDensity(NoiseSpectralDensity / 2); };
	void setNoiseSpectralDensity(t_real NoiseSpectralDensity) { B13.setNoiseSpectralDensity(NoiseSpectralDensity/2); B14.setNoiseSpectralDensity(NoiseSpectralDensity/2);};
	
	void setNoiseSamplingPeriod(t_real SamplingPeriod) { B7.setSamplingPeriod(SamplingPeriod); B8.setSamplingPeriod(SamplingPeriod); B13.setSamplingPeriod(SamplingPeriod); B14.setSamplingPeriod(SamplingPeriod); };
	void setSeeds(array<int,2> noiseSeeds) { B13.setSeed(noiseSeeds[0]); B14.setSeed(noiseSeeds[1]); };
	void setSeedType(SeedType seedType) { B13.setSeedType(seedType); B14.setSeedType(seedType); };

	// Pulse shaper config
	void setImpulseResponseTimeLength(int impResponseTimeLength) { B19.setImpulseResponseTimeLength(impResponseTimeLength); B20.setImpulseResponseTimeLength(impResponseTimeLength); };
	void setFilterType(PulseShaperFilter fType) { B19.setFilterType(fType); B20.setFilterType(fType); };
	void setRollOffFactor(double rOffFactor) { B19.setRollOffFactor(rOffFactor); B20.setRollOffFactor(rOffFactor); };
	void usePassiveFilterMode(bool pFilterMode) { B19.usePassiveFilterMode(pFilterMode); B20.usePassiveFilterMode(pFilterMode); }
	void setImpulseResponseFilename(string fName) { B19.setImpulseResponseFilename(fName); B20.setImpulseResponseFilename(fName); }
	void setFirstFilteredValueToBeSaved(int value) { HMD24.setFirstValueToBeSaved(value); HMD25.setFirstValueToBeSaved(value);}; // SeeBeginningOfImpulseResponse does not remove
	int getFirstFilteredValueToBeSaved() { HMD24.getFirstValueToBeSaved(); };													 // all unwanted values from the signal.

	//void setClockPeriod(double per) { B13A.setClockPeriod(per); B13B.setClockPeriod(per); };

	// Sampler config
	void setSamplesToSkip(int sToSkip) { B22.setSamplesToSkip(sToSkip); B23.setSamplesToSkip(sToSkip); };
	void setSamplerOpticalPower_dBm(double optPower_dBm) { B22.setOutputOpticalPower_dBm(optPower_dBm); B23.setOutputOpticalPower_dBm(optPower_dBm); };
	
	// SNR estimators config
	void setSNRFilename(string fname) { B18.setFilename(fname); }
	void setFilteredSNRFilename(string fname) { B21.setFilename(fname); }
	void setSNRRollOffComp(double rollOff) { B18.setRollOff(rollOff); B21.setRollOff(rollOff);}
	void setSNRnoiseBw(double nBw) { B18.setNoiseBw(nBw); B21.setNoiseBw(nBw);}
	void setSNR1method(SNREstimatorMethod mtd) { B18.setEstimatorMethod(mtd); }
	void setSNR2method(SNREstimatorMethod mtd) { B21.setEstimatorMethod(mtd); }
	void setIgnoreInitialSamples1(int iis) { B18.setIgnoreInitialSamples(iis); }
	void setIgnoreInitialSamples2(int iis) { B21.setIgnoreInitialSamples(iis); }
	void setSegmentSize(int sSz) { B18.setSegmentSize(sSz); B21.setSegmentSize(sSz); }
	void setOverlapPercent(double olp) { B18.setOverlapPercent(olp); B21.setOverlapPercent(olp);}

	// Power Spectral Density Config
	void setPSDFilename(string fname) { B17.setFilename(fname); }
	string getPSDFilename(void) { return B17.getFilename(); }
	void setPSDMeasuredIntervalSize(int msz) { B17.setMeasuredIntervalSize(msz); }
	int getPSDMeasuredIntervalSize(void) { B17.getMeasuredIntervalSize(); }
	void setPSDSegmentSize(int msz) { B17.setSegmentSize(msz); }
	int getPSDSegmentSize(void) { B17.getSegmentSize(); }
	void setPSDOverlapPercent(double olp) { B17.setOverlapPercent(olp); }
	double getPSDOverlapPercent(void) { B17.getOverlapPercent(); }
	void setPSDConfInterval(double P) { B17.setConfidence(P); }
	double getPSDConfInterval(void) { B17.getConfidence(); }
	
	// Electrical Filter config
	void setCutoffFrequency(double ctfFreq) { B11.setCutoffFrequency(ctfFreq); B12.setCutoffFrequency(ctfFreq); }
	void setElFilterType(Filter fType) { B11.setFilterType(fType); B12.setFilterType(fType); };
	void setElFilterImpulseResponse(vector<t_real> ir) { B11.setImpulseResponse(ir); B12.setImpulseResponse(ir);; }
	void setElFilterImpulseResponseLength(int irl) { B11.setImpulseResponseTimeLength(irl);  B12.setImpulseResponseTimeLength(irl); }
//	void setFirstFilteredValueToBeSavedEl(int value) { HMD14.setFirstValueToBeSaved(value); HMD15.setFirstValueToBeSaved(value);}; // SeeBeginningOfImpulseResponse does not remove
//	int getFirstFilteredValueToBeSavedEl() { HMD14.getFirstValueToBeSaved(); };													 // all unwanted values from the signal.

};

#endif

# ifndef PROGRAM_INCLUDE_EDFA_H_
# define PROGRAM_INCLUDE_EDFA_H_

# include "netxpto_20180118.h"
# include "add_20171116.h"
# include "white_noise_20180420.h"
# include "super_block_interface_20180118.h"
# include "ideal_amplifier_20180118.h"
# include <math.h>

class Edfa : public SuperBlock {
	
	OpticalSignal EDFA1{ "EDFA1.sgn" };   // Amplified signal
	OpticalSignal EDFA2{ "EDFA2.sgn" };   // Noise

	IdealAmplifier B1;
	WhiteNoise B2;
	Add B3;
	
	public:
		Edfa(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal);

		void setGain_dB(t_real newGain) {
			powerGain_dB = newGain;
			B1.setGain(sqrt(pow(10, (powerGain_dB / 10))));
			B2.setNoiseSpectralDensity(PLANCK_CONSTANT*SPEED_OF_LIGHT*(1 / wavelength)*(pow(10, powerGain_dB / 10) - 1) * pow(10, noiseFigure / 10) / 2);
		}

		t_real getGain_dB(void) { return powerGain_dB; }

		void setNoiseFigure(t_real newNoiseFigure) {
			noiseFigure = newNoiseFigure;
			B2.setNoiseSpectralDensity(PLANCK_CONSTANT*SPEED_OF_LIGHT*(1 / wavelength)*(pow(10, powerGain_dB / 10) - 1) * pow(10, noiseFigure / 10) / 2);
		}
		t_real getNoiseFigure(void) { return noiseFigure; }
		
		void setNoiseSamplingPeriod(t_real newSamplingPeriod) {
			samplingPeriod = newSamplingPeriod;
			B2.setSamplingPeriod(samplingPeriod); }
		t_real getNoiseSamplingPeriod(void) { return samplingPeriod; }

		void setWavelength(t_real newWavelength) {
			wavelength = newWavelength;
			B2.setNoiseSpectralDensity(PLANCK_CONSTANT*SPEED_OF_LIGHT*(1 / wavelength)*(pow(10, powerGain_dB / 10) - 1) * pow(10, noiseFigure / 10) / 2);
		}
		t_real getWavelength(void) { return wavelength; }

		void setDirName(string newDirName);
		string getDirName(void) { return dirName; }

	private:

		t_real powerGain_dB{10};
		t_real noiseFigure{4};
		t_real wavelength{ 1550.1e-9};
		t_real samplingPeriod{ 1 };
		string dirName{""};
};

# endif // PROGRAM_INCLUDE_EDFA_H_

//  version: 20180302
//   author: Diamantino Silva
// whatsnew: outputs the number of detected photons

# ifndef PROGRAM_INCLUDE_PHOTOELECTRON_GENERATOR_H_
# define PROGRAM_INCLUDE_PHOTOELECTRON_GENERATOR_H_

# include "netxpto_20180118.h"
#include <random>

// Simulates a photodiode
class PhotoelectronGenerator : public Block {

	bool firstTime{ true };
	int aux = 0;
	double t = 0;

	double efficiency{ 1.0 };

	// Quantum shot noise
	bool shotNoise{ false };
	
	// Thermal Noise (for future versions)
	t_real thermalNoiseAmplitude{ 0.0 };
	
	// Random number generator for quantum shot noise
	default_random_engine generatorAmp;


public:

	PhotoelectronGenerator() {};
	PhotoelectronGenerator(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);

	void setEfficiency(double efficiency_new) { efficiency = efficiency_new; }
	double const getEfficiency(void) { return efficiency; }
	
	// Shot Noise
	void setShotNoise(bool sNoise) { shotNoise = sNoise; }
	bool getShotNoise(void) { return shotNoise; }

	// Thermal Noise
	void setThermalNoiseAmplitude(double amplitude) { thermalNoiseAmplitude = amplitude; }
	double getThermalNoiseAmplitude(void) { return thermalNoiseAmplitude; };

};

#endif // !PROGRAM_INCLUDE_PHOTODIODE_H_

// version: 2018-02-21
//  author: Diamantino Silva

# ifndef PROGRAM_INCLUDE_PHOTODIODE_H_
# define PROGRAM_INCLUDE_PHOTODIODE_H_

# include "netxpto_20180118.h"
#include <random>

// Simulates a photodiode
class Photodiode : public Block {

	bool firstTime{ true };
	int aux = 0;
	double t = 0;

	double efficiency{ 1 };

	bool shotNoise{ false };

	t_real thermalNoiseAmplitude{ 0.0 };

	default_random_engine generatorAmp;


public:

	Photodiode() {};
	Photodiode(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
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

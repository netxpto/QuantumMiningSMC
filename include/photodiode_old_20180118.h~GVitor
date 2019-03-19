# ifndef PROGRAM_INCLUDE_PHOTODIODE_OLD_H_
# define PROGRAM_INCLUDE_PHOTODIODE_OLD_H_

# include "netxpto_20180118.h"
#include <random>

// Simulates a photodiode
class Photodiode : public Block {

	bool firstTime{ true };
	int aux = 0;
	bool firstPass{ true };
	double t = 0;

public:

	double frequencyMismatch{ 1.9441e+11 };
	double responsivity = 1;
	bool shotNoise = false;

	bool thermalNoise = false;
	t_real thermalNoiseAmplitude = 4.7626e-06;

	default_random_engine generatorAmp1;
	default_random_engine generatorAmp2;

	Photodiode() {};
	Photodiode(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);

	void setResponsivity(t_real Responsivity) { responsivity = Responsivity; }
	double const getResponsivity(void) { return responsivity; }

	void useNoise(bool sNoise) { shotNoise = sNoise; }

	void useThermalNoise(bool sNoise) { thermalNoise = sNoise; }
	void setThermalNoiseAmplitude(double amplitude) { thermalNoiseAmplitude = amplitude; }
	double const getThermalNoiseAmplitude(void) { return thermalNoiseAmplitude; };

private:
};


#endif // !PROGRAM_INCLUDE_PHOTODIODE_OLD_H_

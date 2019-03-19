# ifndef SINGLE_PHOTON_DETECTOR_H_
# define SINGLE_PHOTON_DETECTOR_H_

# include <random>
# include <algorithm>  // min()
# include <math.h>	   // remainder(), pow()
# include <chrono>

# include "netxpto_20180418.h"

class SinglePhotonDetector : public Block {
	
	// Input Parameters
	t_integer path{ 0 };
	std::default_random_engine generator;
	double probabilityDarkCount{ 0.0 };
	int out{ 0 };
	int samplesPerSymbol{ 1 };
	bool firstTime{ true };

public:

	SinglePhotonDetector() {};
	SinglePhotonDetector(vector<Signal *> &inputSignals, vector<Signal *> &outputSignals) : Block(inputSignals, outputSignals){};

	void initialize(void);

	bool runBlock(void);

	void setPath(t_integer p) { path = p; };
	t_integer getPath() { return path; };

	void setProbabilityDarkCount(double pdarkcount) { probabilityDarkCount = pdarkcount; };
	double getProbabilityDarkCount() { return probabilityDarkCount; };

	
};

#endif

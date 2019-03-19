# ifndef PROGRAM_INCLUDE_FREQUENCY_MISMATCH_COMPENSATION_H_
# define PROGRAM_INCLUDE_FREQUENCY_MISMATCH_COMPENSATION_H_


# include "netxpto_20180418.h"

# include <complex>
# include <random>
# include <vector>

enum FrequencyCompensationMode { PilotAided, BlindEstimation, SpectralMethod };
enum PilotMode{ Sequential, Delay };

class FrequencyMismatchCompensation : public Block {

	/* State Variables */


	/* Input Parameters */

	int pilotRate = 2;

	bool firstRun = true;
	long int auxP = 0;
	long int auxQ = 0;
	long int count = 0;
	double theta = 0;

	FrequencyCompensationMode frequencyMode{ PilotAided };
	PilotMode pilotMode{ Sequential };

	int numberOfSamplesForEstimation = 20 + 1;
	double frequencyEstimate;

	vector<t_complex> samplesForEstimation;
	vector<t_complex> delayLine;

public:

	/* Methods */
	FrequencyMismatchCompensation() {};
	FrequencyMismatchCompensation(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);

	void setFrequencyCompensationMode(FrequencyCompensationMode fMode) { frequencyMode = fMode; }
	FrequencyCompensationMode const getFrequencyCompensationMode(void) { return frequencyMode; }

	void setPilotMode(PilotMode pMode) { pilotMode = pMode; }
	PilotMode const getPilotMode(void) { return pilotMode; }

	void setPilotRate(int pRate) { pilotRate = pRate; }
	double getPilotRate() { return pilotRate; }

	void setNumberOfSamplesForEstimation(int nSamplesEstimation) { numberOfSamplesForEstimation = nSamplesEstimation; samplesForEstimation.resize(nSamplesEstimation); delayLine.resize(nSamplesEstimation);
	};
	int const getNumberOfSamplesForEstimation(void) { return numberOfSamplesForEstimation; };

};

#endif 

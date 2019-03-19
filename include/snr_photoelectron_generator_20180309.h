// UNDER CONSTRUCTION!!!
# ifndef SNR_PHOTOELECTRON_GENERATOR_H_
# define SNR_PHOTOELECTRON_GENERATOR_H_

# include "netxpto_20180118.h"
# include <vector>

// Calculates the signal to noise ratio (SNR) of photoelectron generator
class SnrPhotoelectronGenerator : public Block {
	
	bool firstTime{ true };
	
public:
	
	SnrPhotoelectronGenerator(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig,OutputSig){};
	
	// General block API
	void initialize(void);
	bool runBlock(void);
	void terminate(void);
	
	// API to tWindow
	void setTWindow(double tWindow_in) { tWindow = tWindow_in; }
	double const setTWindow(void) { return tWindow; }
	
private:
	
	// General block parameters
	int firstPass = 1;
	
	// snr_photoelectron_generator parameters.
	
	// Time window length.
	double tWindow = 0.0;
	
	
	// State variables

	// Index of current window
	long int currentWindowIndex = 0;
	
	// Last window final sample index.
	long int lastWindowSample  = 0;
	long int firstWindowSample = 0;
	// Number of samples of the current window
	long int nWindowSamples = 0;
	// Current sample of the current window
	long int currentWindowSample = 0;
	
	
	// Average number of samples per window.
	double averageNSamplesPerWindow = 0.0;
	
	
	
	// Auxiliary variables for calculating the signal's first and second moment.
	double aux_sum1 = 0.0;
	double aux_sum2 = 0.0;
};

#endif // !PROGRAM_SNR_PHOTOELECTRON_GENERATOR_H_

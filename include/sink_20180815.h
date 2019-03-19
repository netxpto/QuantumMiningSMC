# ifndef SINK_H_
# define SINK_H_

# include "netxpto_20180815.h"

class Sink : public Block {

public:

	// ##########################################################################

	Sink(initializer_list<Signal*> InputSig, initializer_list<Signal*> OutputSig) : Block(InputSig, OutputSig) {};
	bool runBlock(void);

	// ##########################################################################

	void setNumberOfSamples(long int nOfSamples) { numberOfSamples = nOfSamples; };
	long int getNumberOfSamples() const { return numberOfSamples; };

	void setProcessedSamples(long int pSamples) { processedSamples = pSamples; };
	long int getProcessedSamples() const { return processedSamples; };

	void setDisplayNumberOfSamples(bool opt) { displayNumberOfSamples = opt; };
	bool getDisplayNumberOfSamples() const { return displayNumberOfSamples; };

	// ###########################################################################
	
private:

	// # Input Parameters ########################################################

	long int numberOfSamples{ -1 };
	long int processedSamples{ 0 };
	bool displayNumberOfSamples{ true };

	// ###########################################################################
};

#endif

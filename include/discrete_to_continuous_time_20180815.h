# ifndef DISCRETE_TO_CONTINUOUS_TIME_H_
# define DISCRETE_TO_CONTINUOUS_TIME_H_

# include "../include/netxpto_20180815.h"

class DiscreteToContinuousTime : public Block {

public:

	//################################################################################################################################

	DiscreteToContinuousTime(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};
	//DiscreteToContinuousTime(vector<Signal *> &inputSignals, vector<Signal *> &outputSignals) :Block(inputSignals, outputSignals){};
	//DiscreteToContinuousTime() {};

	void initialize(void);
	bool runBlock(void);

	//################################################################################################################################
		
	void setNumberOfSamplesPerSymbol(int nSamplesPerSymbol){ numberOfSamplesPerSymbol = nSamplesPerSymbol; };
	int const getNumberOfSamplesPerSymbol(void){ return numberOfSamplesPerSymbol; };

	//################################################################################################################################

private:

	/* input parameters */
	int numberOfSamplesPerSymbol{ 8 };

	/* state variables */
	int index{ 0 };

	//int contador{ 0 };

};

#endif

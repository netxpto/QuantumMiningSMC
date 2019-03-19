# ifndef DISCRETE_TO_CONTINUOUS_TIME_H_
# define DISCRETE_TO_CONTINUOUS_TIME_H_

class DiscreteToContinuousTime : public Block {

	/* input parameters */
	int numberOfSamplesPerSymbol{ 8 };

	/* state variables */
	int index{ 0 };

public:
	DiscreteToContinuousTime() {};
	DiscreteToContinuousTime(vector<Signal *> &inputSignals, vector<Signal *> &outputSignals) :Block(inputSignals, outputSignals){};

	void initialize(void);

	bool runBlock(void);
		
	void setNumberOfSamplesPerSymbol(int nSamplesPerSymbol){ numberOfSamplesPerSymbol = nSamplesPerSymbol; };
	int const getNumberOfSamplesPerSymbol(void){ return numberOfSamplesPerSymbol; };

	int contador{ 0 };
};

#endif
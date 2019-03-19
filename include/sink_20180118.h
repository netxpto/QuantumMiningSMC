# ifndef SINK_H_
# define SINK_H_

# include <algorithm>   // std::min
# include <string>		// compare

# include "netxpto_20180418.h"

class Sink : public Block {


public:

	/* State Variables */

	long int numberOfSamples{ -1 };

	/* Methods */
	Sink() {};
	Sink(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);

	bool runBlock(void);

	void setNumberOfSamples(long int nOfSamples){ numberOfSamples = nOfSamples; };

	void setDisplayNumberOfSamples(bool opt) { displayNumberOfSamples = opt; };

private: 

	/* Input Parameters */

	bool displayNumberOfSamples{ true };


};

#endif

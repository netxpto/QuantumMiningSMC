# ifndef SINKSTRING_H_
# define SINKSTRING_H_

# include <algorithm>   // std::min
# include <string>		// compare

# include "netxpto_20180118.h"

class SinkString : public Block {

	/* State Variables */

	bool displayNumberOfSamples{ false };

public:

	/* Input Parameters */

	long int numberOfSamples{ -1 };

	/* Methods */
	SinkString() {};
	SinkString(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);

	bool runBlock(void);

	void setNumberOfSamples(long int nOfSamples){ numberOfSamples = nOfSamples; };

	void setDisplayNumberOfSamples(bool opt) { displayNumberOfSamples = opt; };

};

#endif

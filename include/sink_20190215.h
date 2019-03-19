# ifndef SINK_H_
# define SINK_H_

# include <algorithm>   // std::min
# include <string>		// compare

# include "netxpto_20190215.h"

class Sink : public Block {

	/* State Variables */

	bool displayNumberOfSamples{ false };

public:

	/* Input Parameters */

	long int numberOfSamples{ -1 };

	/* Methods */
	Sink() {};
	Sink(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);

	bool runBlock(void);

	void setNumberOfSamples(long int nOfSamples){ numberOfSamples = nOfSamples; };

	void setDisplayNumberOfSamples(bool opt) { displayNumberOfSamples = opt; };

};

#endif

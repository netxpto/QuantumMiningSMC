# ifndef PROGRAM_INCLUDE_MATCHED_FILTER_H_
# define PROGRAM_INCLUDE_MATCHED_FILTER_H_


# include "netxpto_20180815.h"

# include <complex>
# include <vector>
# include "../algorithms/fft/fft_20180208.h"

class MatchedFilter : public Block {

	/* State Variables */


	/* Input Parameters */

	int aux = 0;
	int count = 0;
	int runCounter = 0;
	int blockSize;
	int numberOfTaps = 64;
	double rollOffFactor = 0.5;

	vector<t_complex> delayLine;
	vector<t_complex> delayLinePrevious;
	vector<t_real> impulseResponse;
	vector<t_complex> filter;
	vector<t_complex> filterShifted;
	vector<t_complex> filtered;
	vector<t_complex> outVector;

public:

	/* Methods */
	MatchedFilter(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setNumberOfTaps(int nTaps) { numberOfTaps = nTaps; }
	int const getNumberOfTaps(void) { return numberOfTaps; }

	void setRollOffFactor(double rOffFactor) { rollOffFactor = rOffFactor; }
	double const getBlockSize(void) { return rollOffFactor; }



};

#endif 

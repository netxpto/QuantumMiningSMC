# ifndef PROGRAM_INCLUDE_DC_COMPONENT_REMOVAL_H_
# define PROGRAM_INCLUDE_DC_COMPONENT_REMOVAL_H_


# include "netxpto_20180815.h"

# include <complex>
# include <random>
# include <vector>

enum dcRemovalType{ Average, MovingAverage };

class DCComponentRemoval : public Block {

	/* State Variables */


	/* Input Parameters */

	int aux = 0;
	int count = 0;
	bool flag{ false };
	int blockSize;
	dcRemovalType type = dcRemovalType::Average;

	vector<t_complex> delayLine;
	vector<t_complex> delayLinePrevious;
	vector<t_complex> finiteImpulseResponse;
	vector<t_complex> filtered;
	vector<t_complex> outVector;

	t_complex mean;


public:

	/* Methods */
	DCComponentRemoval(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	/*void setBlockSize(int bSize) { blockSize = bSize; }
	int const getBlockSize(void) { return blockSize; }*/



};

#endif 

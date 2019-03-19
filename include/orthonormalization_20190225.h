# ifndef PROGRAM_INCLUDE_ORTHONORMALIZATION_H_
# define PROGRAM_INCLUDE_ORTHONORMALIZATION_H_


# include "netxpto_20180815.h"

# include <complex>
# include <random>
# include <vector>

class Orthonormalization : public Block {

	/* State Variables */


	/* Input Parameters */

	int aux = 0;
	int count = 0;
	bool flag{ false };
	int blockSize;

	vector<double> delayLineI;
	vector<double> delayLineQ;
	vector<double> delayLineQog;

	double PI;
	double PX;
	double PQ;


public:

	/* Methods */
	Orthonormalization(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	/*void setBlockSize(int bSize) { blockSize = bSize; }
	int const getBlockSize(void) { return blockSize; }*/

};

#endif 

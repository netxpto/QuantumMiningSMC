# ifndef PROGRAM_INCLUDE_TIMING_DESKEW_H_
# define PROGRAM_INCLUDE_TIMING_DESKEW_H_


# include "netxpto_20180815.h"

# include <complex>
# include <random>
# include <vector>


class TimingDeskew : public Block {

	/* State Variables */


	/* Input Parameters */

	int aux = 0;
	int count = 0;


	int blockSize;
	vector<t_complex> delayLineI;
	vector<t_complex> delayLineQ;

	vector<t_complex> delayLineIOut;
	vector<t_complex> delayLineQOut;

	vector<t_complex> delayLineIPrevious;
	vector<t_complex> delayLineQPrevious;

	vector<t_complex> filterI;
	vector<t_complex> filterQ;

	vector<t_complex> realFiltered;
	vector<t_complex> imagFiltered;

	vector<t_complex> outVector;

	vector<double> skew;
	vector<double> f;
	bool firstRun{ true };


public:

	/* Methods */
	TimingDeskew(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	/*void setBlockSize(int bSize) { blockSize = bSize; }
	int const getBlockSize(void) { return blockSize; };*/

	void setSkew(vector<double> s) { skew.resize(s.size()); skew[0] = s[0]; skew[1] = s[1]; }
	//vector<double> const getSkew(void) { return skew; };

};

#endif 

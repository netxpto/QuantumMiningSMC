# ifndef M_QAM_MAPPER_H_
# define M_QAM_MAPPER_H_

# include <vector>	     // vector container
# include <math.h>       // log2 
# include <complex>
# include <fstream>
# include <iostream>
# include <math.h>
# include <stdio.h>
# include <string>
# include <strstream>
# include <vector>
# include <algorithm>

# include "netxpto_20180418.h"

using namespace std;

/*struct t_iqValues {
	t_real i;
	t_real q;
};*/ // eliminar 2017-01-30

/* Realizes the M-QAM mapping. */
class MQamMapper : public Block {

	/* State Variables */

	t_integer auxBinaryValue{ 0 };
	t_integer auxSignalNumber{ 0 };


public:

	/* Input Parameters */

	t_integer m{ 4 };
	vector<t_iqValues> iqAmplitudes{{ 1, 1 }, { -1, 1 }, { -1, -1 }, { 1, -1 } };

	/* Methods */
	MQamMapper() {};
	MQamMapper(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);

	bool runBlock(void);

	void setM(int mValue);		// m should be of the form m = 2^n, with n integer;

	void setIqAmplitudes(vector<t_iqValues> iqAmplitudesValues);

};

#endif

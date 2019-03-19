# ifndef PROGRAM_INCLUDE_CLONER_H_
# define PROGRAM_INCLUDE_CLONER_H_


# include "netxpto_20180418.h"

# include <complex>
# include <random>
# include <vector>

class Cloner : public Block {

	/* State Variables */


	/* Input Parameters */

public:

	/* Methods */
	Cloner() {};
	Cloner(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);

};

#endif 

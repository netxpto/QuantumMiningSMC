# ifndef PROGRAM_INCLUDE_REAL_TO_COMPLEX_H_
# define PROGRAM_INCLUDE_REAL_TO_COMPLEX_H_

# include "netxpto_20180418.h"


// Creates a complex signal from two real signals
class RealToComplex2 : public Block {

	/* State Variables */
	bool firstTime{ true };

 public:

	/* Methods */
	RealToComplex2() {};
	RealToComplex2(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :
		Block(InputSig, OutputSig){};

	void initialize(void);
	bool runBlock(void);

};

# endif // PROGRAM_INCLUDE_REAL_TO_COMPLEX_H_

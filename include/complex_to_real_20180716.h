# ifndef PROGRAM_INCLUDE_COMPLEX_TO_REAL_H_
# define PROGRAM_INCLUDE_COMPLEX_TO_REAL_H_

# include "netxpto_20180418.h"


// Creates two real signals from a complex signal
class ComplexToReal : public Block {

	/* State Variables */
	bool firstTime{ true };

 public:

	/* Methods */
	 ComplexToReal() {};
	 ComplexToReal(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :
		Block(InputSig, OutputSig){};

	void initialize(void);
	bool runBlock(void);

};

# endif // PROGRAM_INCLUDE_REAL_TO_COMPLEX_H_

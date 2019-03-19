# ifndef PROGRAM_INCLUDE_COMPLEX_TO_REALH_
# define PROGRAM_INCLUDE_COMPLEX_TO_REAL_H_

# include "netxpto.h"


// Creates a complex signal from two real signals
class ComplexToReal : public Block {

	/* State Variables */
	bool firstTime{ true };

public:

	/* Methods */
	ComplexToReal() {};
	ComplexToReal(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :
		Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

};

# endif // PROGRAM_INCLUDE_REAL_TO_COMPLEX_H_
#pragma once

# ifndef PROGRAM_INCLUDE_REAL_TO_COMPLEX_H_
# define PROGRAM_INCLUDE_REAL_TO_COMPLEX_H_

# include "netxpto.h"


// Creates a complex signal from two real signals
class RealToComplex2 : public Block {

	/* State Variables */
	bool firstTime{ true };
	double samplingPeriod{ 0.0 };

 public:

	/* Methods */
	RealToComplex2() {};
	RealToComplex2(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :
		Block(InputSig, OutputSig){};

	void initialize(void);
	bool runBlock(void);
	void setSamplingPeriod(double sPeriod) { samplingPeriod = sPeriod; }

};

# endif // PROGRAM_INCLUDE_REAL_TO_COMPLEX_H_

#pragma once
# ifndef PROGRAM_INCLUDE_IQ_IDEAL_H_
# define PROGRAM_INCLUDE_IQ_IDEAL_H_

# include <math.h>       /* pow */
# include "netxpto.h"

using namespace std;

enum modulator { ideal, mzm};


// Implements a IQ modulator.
class IqModulator : public Block {

	/* State Variables */

	bool firstTime{ true };
	modulator modulatorType{ ideal };
	double vpi = 3.75;

public:

	/* Input Parameters */


	/* Methods */
	IqModulator() {};
	IqModulator(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);


	void setModulatorType(modulator fType) { modulatorType = fType; };
	modulator const getModulatorType(void) { return modulatorType; };

	void setVpi(double v_pi) { vpi = v_pi; };
	double const getVpi(void) { return vpi; };


};

# endif // PROGRAM_INCLUDE_netxpto_H_



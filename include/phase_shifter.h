# ifndef PROGRAM_INCLUDE_PHASESHIFTER_H_
# define PROGRAM_INCLUDE_PHASESHIFTER_H_

# include "netxpto.h"

// Introduces a 90 degree phase shift in the signal
class PhaseShifter : public Block {

	bool firstTime{ true };

public:
	void setrfAmplitude(double rfAmpl) { rfAmplitude = rfAmpl; }
	double getRfAmplitude() { return rfAmplitude; } // This is for scaling purposes

	PhaseShifter(){};
	PhaseShifter(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);


private:
	double rfAmplitude{ 1.0 };

};


#endif 

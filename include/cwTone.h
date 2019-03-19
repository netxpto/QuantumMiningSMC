# ifndef PROGRAM_INCLUDE_CWTONE_H_
# define PROGRAM_INCLUDE_CWTONE_H_

# include "netxpto.h"

class CwTone : public Block {

	bool firstTime{ true };

public:

	CwTone() {};
	CwTone(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};
	void initialize(void);
	bool runBlock(void);

	void setCwFrequency(double Freq){ cwFrequency = Freq; }
	void setCwAmplitude(double Amp) { cwAmplitude = Amp; }
	void setPhase(double iniatialPhase) { cwPhase = iniatialPhase; }

private:
	double cwAmplitude{500};   // Amplitude
	double cwFrequency{6e9};   // Bandwidth
	double cwPhase{0};		   // Phase

};

#endif




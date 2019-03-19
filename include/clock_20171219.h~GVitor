# ifndef CLOCK_H_
# define CLOCK_H_


# include "netxpto_20180118.h"

class Clock : public Block {


	/* input parameters */
	double period{0.0};
	double phase{0.0};
	double samplingPeriod{0.0};

	/* state variables */
	int index{0};
	int phaseShift{0};

	
public:

	Clock() {};
	Clock(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setClockPeriod(double per) { period = per; }
	double getClockPeriod() { return period; }

	void setClockPhase(double pha) { phase = pha; }
	double getClockPhase() { return phase; }

	void setSamplingPeriod(double sPeriod) { samplingPeriod = sPeriod; }
	double getSamplingPeriod() { return samplingPeriod; }

};

# endif

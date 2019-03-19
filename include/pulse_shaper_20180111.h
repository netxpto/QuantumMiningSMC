# ifndef PULSE_SHAPER_H_
# define PULSE_SHAPER_H_

# include <vector>
# include "netxpto_20180418.h"

using namespace std;

enum PulseShaperFilter { RaisedCosine, Gaussian, Square };


class PulseShaper : public FIR_Filter{

	/* Input Parameters */
	PulseShaperFilter filterType{ RaisedCosine };

	int impulseResponseTimeLength{ 16 };				// in units of symbol period

	double rollOffFactor{ 0.9 };						// Roll-off factor (roll) for the raised-cosine filter

	bool passiveFilterMode{ false };

	double pulseWidth{ 1.0 };								// in units of time (ms)

	double pulseDelay{ 0.0 };								// in units of time (ms)


public:

	/* Methods */
	PulseShaper() :FIR_Filter(){};
	PulseShaper(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :FIR_Filter(InputSig, OutputSig){};

	void initialize(void);
	
	void setImpulseResponseTimeLength(int impResponseTimeLength){ impulseResponseTimeLength = impResponseTimeLength; };
	int const getImpulseResponseTimeLength(void) { return impulseResponseTimeLength; };

	void setFilterType(PulseShaperFilter fType){ filterType = fType; };
	PulseShaperFilter const getFilterType(void){ return filterType; };

	void setRollOffFactor(double rOffFactor){ rollOffFactor = rOffFactor; };
	double const getRollOffFactor(){ return rollOffFactor; };

	void usePassiveFilterMode(bool pFilterMode){ passiveFilterMode = pFilterMode; };

	void setImpulseResponseLength(int impLength) { impulseResponseLength = impLength; };
	int getImpulseResponseLength() { return impulseResponseLength; };

	void setPulseWidth(double pWidth) { pulseWidth = pWidth; };
	double getPulseWidth() { return pulseWidth; };

	void setPulseDelay(double pDelay) { pulseDelay = pDelay; };
	double getPulseDelay() { return pulseDelay; };
		
};

# endif



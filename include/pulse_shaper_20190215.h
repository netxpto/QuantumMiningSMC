# ifndef PULSE_SHAPER_H_
# define PULSE_SHAPER_H_

# include <vector>
# include "netxpto_20190215.h"

using namespace std;

enum PulseShaperFilter { RaisedCosine, Gaussian, Square, RootRaisedCosine };

/* Raised-cosine filter FIR implementation. */
class PulseShaper : public FIR_Filter{

	/* Input Parameters */
	PulseShaperFilter filterType{ RaisedCosine };

	int impulseResponseTimeLength{ 16 };				// in units of symbol period

	double rollOffFactor{ 0.9 };						// Roll-off factor (roll) for the raised-cosine filter

	bool passiveFilterMode{ false };

	bool rrcNormalizeEnergy{ false };                   // Set filter energy to 1, otherwise amplitude == 1

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

	void setRrcNormalizeEnergy(bool ne) { rrcNormalizeEnergy = ne; };
	bool getRrcNormalizeEnergy(void) { return rrcNormalizeEnergy; };

	void usePassiveFilterMode(bool pFilterMode){ passiveFilterMode = pFilterMode; };
		
};

# endif



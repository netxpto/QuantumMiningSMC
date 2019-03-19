# ifndef PULSE_SHAPER_H_
# define PULSE_SHAPER_H_

# include <vector>
# include "netxpto.h"
# include "filter_20180306.h"
# include "fft_20180208.h"
//# include "overlap_save_20180208.h"

using namespace std;

enum PulseShaperFilter { RaisedCosine, RootRaisedCosine, Gaussian, Square };

/* Raised-cosine filter FIR implementation. */
class PulseShaper : public FIR_Filter{

	/* Input Parameters */
	PulseShaperFilter filterType{ RaisedCosine };

	int impulseResponseTimeLength{ 16 };				// in units of symbol period

	double rollOffFactor{ 0.1 };						// Roll-off factor (roll) for the raised-cosine filter

	double BTs{ 0.5 };									// Bandwidth-symbil time product of gaussian filter

	bool passiveFilterMode{ false };

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

	void setBTs(double bandwidthSymbolTimeProduct) { BTs = bandwidthSymbolTimeProduct; };
	double const getBTs() { return BTs; };

	void usePassiveFilterMode(bool pFilterMode){ passiveFilterMode = pFilterMode; };
		
};

# endif



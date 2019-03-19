# ifndef PULSE_SHAPER_FD_20180306_H_
# define PULSE_SHAPER_FD_20180306_H_

# include <vector>
# include "netxpto.h"
# include "filter_20180306.h"
# include "fft_20180208.h"

using namespace std;

enum PulseShaperFilterIR { RaisedCosineIR, RootRaisedCosineIR, GaussianIR, SquareIR };

/* Raised-cosine filter FIR implementation. */
class PulseShaperFd : public FD_Filter {

	/* Input Parameters */
	PulseShaperFilterIR filterTypeIR{ RaisedCosineIR };
	string filterSymmetryTypeIr{"Symmetric" };
	int impulseResponseTimeLength{ 16 };				// in units of symbol period

	double rollOffFactor{ 0.9 };						// Roll-off factor (roll) for the raised-cosine filter

	double BTs{ 0.5 };									// Bandwidth-symbil time product of gaussian filter

	bool passiveFilterMode{ false };

public:

	/* Methods */
	PulseShaperFd() :FD_Filter() {};
	PulseShaperFd(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :FD_Filter(InputSig, OutputSig) {};

	void initialize(void);

	void setImpulseResponseTimeLength(int impResponseTimeLength) { impulseResponseTimeLength = impResponseTimeLength; };
	int const getImpulseResponseTimeLength(void) { return impulseResponseTimeLength; };

	void setFilterType(PulseShaperFilterIR fType) { filterTypeIR = fType; };
	PulseShaperFilterIR const getFilterType(void) { return filterTypeIR; };

	void setFilterSymmetryTypeIr(string fType) { filterSymmetryTypeIr = fType; };
	string const getFilterSymmetryTypeIr(void) { return filterSymmetryTypeIr; };

	void setRollOffFactor(double rOffFactor) { rollOffFactor = rOffFactor; };
	double const getRollOffFactor() { return rollOffFactor; };

	void setBTs(double bandwidthSymbolTimeProduct) { BTs = bandwidthSymbolTimeProduct; };
	double const getBTs() { return BTs; };

	void usePassiveFilterMode(bool pFilterMode) { passiveFilterMode = pFilterMode; };

};

# endif



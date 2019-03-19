# ifndef PULSE_SHAPER_FD_20181110_H_
# define PULSE_SHAPER_FD_20181110_H_

# include "filter_20180306.h"
# include "fft_20180208.h"

using namespace std;

enum PulseShaperFilterTF20181110 { RaisedCosineTF, RootRaisedCosineTF, GaussianTF };

// Raised-cosine filter frequency implementation. // 2018-03-06 //
class PulseShaperFd20181110 : public FD_Filter_20181110 {

	// Input Parameters
	PulseShaperFilterTF20181110 filterType{ RaisedCosineTF };
	string filterSymmetryTypeTf{ "Antisymmetric" };
	int transferFunctionFrequencyLength{ 1024 };
	double rollOffFactor{ 0.9 };	
	double BTs{ 0.5 };									// Bandwidth-symbol time product of gaussian filter
	bool passiveFilterMode{ false };

public:

	// Methods 
	PulseShaperFd20181110() : FD_Filter_20181110() {};
	PulseShaperFd20181110 (vector<Signal *> &InputSig, vector<Signal *> OutputSig):FD_Filter_20181110(InputSig, OutputSig) {};

	void initialize(void);
	
	void setTransferFunctionLength(int transFunctionLength) { transferFunctionFrequencyLength = transFunctionLength; };
	int const getTransferFunctionLength(void) { return transferFunctionFrequencyLength; };

	void setFilterType(PulseShaperFilterTF20181110 fType) { filterType = fType; };
	PulseShaperFilterTF20181110 const getFilterType(void) { return filterType; };

	void setFilterSymmetryTypeTf(string fType) { filterSymmetryTypeTf = fType; };
	string const getFilterSymmetryTypeTf(void) { return filterSymmetryTypeTf; };

	void setRollOffFactor(double rOffFactor) { rollOffFactor = rOffFactor; };
	double const getRollOffFactor() { return rollOffFactor; };

	void setBTs(double bandwidthSymbolTimeProduct) { BTs = bandwidthSymbolTimeProduct; }; // This term controls the bandwidth of the gaussian filter
	double const getBTs() { return BTs; };

	void usePassiveFilterMode(bool pFilterMode) { passiveFilterMode = pFilterMode; };
};

# endif




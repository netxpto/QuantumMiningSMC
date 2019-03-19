# ifndef  PROGRAM_INCLUDE_FIR_FILTER_H_
# define  PROGRAM_INCLUDE_FIR_FILTER_H_

# include "filter_20180306.h"
# include "fft_20180208.h"

using namespace std;

enum FirFilterType { HilberFilter, DcFilter, AllPass };

// Raised-cosine filter frequency implementation. // 2018-03-06 //
class FirFilter : public FD_Filter_20181110 {

	// Input Parameters
	FirFilterType firFilterType{ AllPass };
	int transferFunctionFrequencyLength{ 128 };
	bool passiveFilterMode{ false };

public:

	// Methods 
	FirFilter() : FD_Filter_20181110() {};
	FirFilter(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :FD_Filter_20181110(InputSig, OutputSig) {};

	void initialize(void);

	void setTransferFunctionFrequencyLength(int transFunctionLength) { transferFunctionFrequencyLength = transFunctionLength; };
	int const getTransferFunctionFrequencyLength(void) { return transferFunctionFrequencyLength; };

	void setFirFilterType(FirFilterType fType) { firFilterType = fType; };
	FirFilterType const getFirFilterType(void) { return firFilterType; };

	void usePassiveFilterMode(bool pFilterMode) { passiveFilterMode = pFilterMode; };
};

# endif



# ifndef PULSE_SHAPER_FD_20180306_H_
# define PULSE_SHAPER_FD_20180306_H_

# include "filter_20180306.h"
# include "fft_20180208.h"

using namespace std;


// Raised-cosine filter frequency implementation. // 2018-03-06 //
class HilbertTransform : public Block {

	// Input Parameters
	int hilbertTransferImpulseResponseLength{ 513 };
	int K{ 0 };
	bool passiveFilterMode{ false };

public:

	// Methods 
	HilbertTransform() ;
	HilbertTransform(vector<Signal *> &InputSig, vector<Signal *> OutputSig){};

	void initialize(void);
	bool runBlock(void);

	void setImpulseResponseTimeLength(int transFunctionLength) { hilbertTransferImpulseResponseLength = transFunctionLength; };
	int const getImpulseResponseTimeLength(void) { return hilbertTransferImpulseResponseLength; };

	void usePassiveFilterMode(bool pFilterMode) { passiveFilterMode = pFilterMode; };
};

# endif



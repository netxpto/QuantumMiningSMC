# ifndef PROGRAM_INCLUDE_BIT_ERROR_RATE_H_
# define PROGRAM_INCLUDE_BIT_ERROR_RATE_H_

# include "netxpto_20180118.h"
# include <vector>

// Calculates the BER between two binary signals
class BitErrorRate : public Block {

	bool firstTime{ true };
	
public:

	BitErrorRate(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig,OutputSig){};
	
	void initialize(void);
	bool runBlock(void);

	double coincidences = 0;
	double receivedBits = 0;

	void setConfidence(double P) { alpha = 1-P; }
	double const getConfidence(void) { return 1 - alpha; }

	void setMidReportSize(int M) { m = M; }
	int const getMidReportSize(void) { return m; }
    
    void setLowestMinorant(double lMinorant) { lowestMinorant=lMinorant; }
    double getLowestMinorant(void) { return lowestMinorant; }

private:
    
    double lowestMinorant = 1e-10;
    
	int firstPass = 1;

	double alpha = 0.05;
	double z;
	int m = 0;
	int n = 0;
};


#endif // !PROGRAM_INCLUDE_BIT_ERROR_RATE_H_

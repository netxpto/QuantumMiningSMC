# ifndef PROGRAM_INCLUDE_BIT_ERROR_RATE_H_
# define PROGRAM_INCLUDE_BIT_ERROR_RATE_H_

# include "../include/netxpto_20180815.h"
# include <vector>
# include <algorithm>
# include <complex>

// Calculates the BER between two binary signals

enum MidReportType{Cumulative, Reset};

class BitErrorRate : public Block {

	bool firstTime{ true };
	
public:

	BitErrorRate(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig,OutputSig){};
	
	void initialize(void);
	bool runBlock(void);

	double coincidences = 0;
	double receivedBits = 0;
	double receivedBits_mid = 0;
	double coincidences_mid = 0;

	void setConfidence(double P) { alpha = 1-P; }
	double const getConfidence(void) { return 1 - alpha; }

	void setMidReportSize(int M) { m = M; }
	int const getMidReportSize(void) { return m; }
    
    void setLowestMinorant(double lMinorant) { lowestMinorant=lMinorant; }
    double getLowestMinorant(void) { return lowestMinorant; }

	void setMidReportType(MidReportType mrt) { midRepType = mrt; };
	MidReportType getMidReportType(void) { return midRepType; };

private:
    
	/*input parameters*/
    double lowestMinorant = 1e-10;
    
	int firstPass = 1;

	double alpha = 0.05;
	double z;
	int m = 0;
	int n = 0;

	MidReportType midRepType { Cumulative };
};


#endif // !PROGRAM_INCLUDE_BIT_ERROR_RATE_H_

# ifndef PROGRAM_INCLUDE_ERROR_VECTOR_MAGNITUDE_H_
# define PROGRAM_INCLUDE_ERROR_VECTOR_MAGNITUDE_H_

# include "../include/netxpto_20180418.h"
# include <vector>
# include <algorithm>
# include <complex>

class ErrorVectorMagnitude : public Block {

public:

	ErrorVectorMagnitude(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig,OutputSig){};
	
	void initialize(void);
	bool runBlock(void);

	double coincidences = 0;
	double receivedBits = 0;
	double receivedBits_mid = 0;
	double coincidences_mid = 0;

	void setReferenceAmplitude(double rAmplitude) { referenceAmplitude = rAmplitude; }
	double getReferenceAmplitude(void) { return referenceAmplitude; }

	void setMidReportSize(int M) { m = M; }
	int getMidReportSize(void) { return m; }

	void setNumberOfSymbols(int nSymbols) { numberOfSymbols = nSymbols; }
	int getNumberOfSymbols(void) { return numberOfSymbols; }

private:
    
	/*input parameters*/
	bool firstPass{ true };
	bool firstTime{ true };
	double EVM = 0;
	double referenceAmplitude = 1.0;
	int numberOfSymbols = 0;
	int n = 0;
	int m = 0;
	int receivedSymbols = 0;
};


#endif // !PROGRAM_INCLUDE_ERROR_VECTOR_MAGNITUDE_H_

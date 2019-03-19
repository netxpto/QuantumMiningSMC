# ifndef PROGRAM_INCLUDE_CarrierPhaseCompensation_H_
# define PROGRAM_INCLUDE_CarrierPhaseCompensation_H_


# include "netxpto_20180118.h"

# include <complex>
# include <random>
# include <vector>

class CarrierPhaseCompensation : public Block {

public:

	/* Methods */
	CarrierPhaseCompensation() {};
	CarrierPhaseCompensation(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	//void setSamplingPeriod(double sPeriod) { samplingPeriod = sPeriod; }
	//void setSymbolPeriod(double sPeriod) { symbolPeriod = sPeriod; }

	void setnTaps(int ntaps) { nTaps = ntaps; }
	double getnTaps() { return nTaps; }

	void setmQAM(int mQAMs) { mQAM = mQAMs; }
	double getmQAM() { return mQAM; }

	void setTestPhase(int nTphase) { nTestPhase = nTphase; }
	double getTestPhase() { return nTestPhase; }

	void setmethodType(string mType) { methodType = mType; }
	string getmethodType() { return methodType; }

	void setBPStype(string tBPS) { BPStype = tBPS; }
	string getBPStype() { return BPStype; }

private:



	/* Input Parameters */
	int nTaps{ 1};
	int mQAM{ 4 };
	
	int nTestPhase{ 64 };
	double phiInt{ 3.14159265358979323846 / 2 };
	
	string methodType{ "VV" };
	string BPStype{ "A" };
	//double samplingPeriod{ 0.0 };
	//double symbolPeriod{ 0.0 };

	/* State Variables */
	vector<t_complex> inBuffer;
	size_t pIn;
	double lastPhase{ 0 };

	bool firstTime{ true };



};

#endif 

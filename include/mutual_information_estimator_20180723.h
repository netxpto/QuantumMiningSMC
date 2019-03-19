#ifndef MUTUAL_INFORMATION_ESTIMATOR_H
#define MUTUAL_INFORMATION_ESTIMATOR_H

# include "netxpto_20180418.h"

class MutualInformationEstimator : public Block {

public:
	/*Public Methods*/
	MutualInformationEstimator() {};
	MutualInformationEstimator(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);
	void setMidReportSize(int M) { m = M; }
	int const getMidReportSize(void) { return m; }
	void setConfidence(double P) { alpha_bounds = 1 - P; }
	double const getConfidence(void) { return 1 - alpha_bounds; }

private:
	/*Input Parameters*/
	int m{ 0 };
	double alpha_bounds = 0.05;
	double z;

	/*State Variables*/
	bool firstPass = true;
	int receivedBits{ 0 };
	double alpha{ 0 };
	double p{ 0 };
	int n{ 0 };

	/*Private Methods*/
	
};

#endif // !MUTUAL_INFORMATION_ESTIMATOR_H


#ifndef  PROBABILITY_ESTIMATOR_H
#define PROBABILITY_ESTIMATOR_H

#include "netxpto_20180118.h"
#include <string>

class ProbabilityEstimator : public Block {


public:

	ProbabilityEstimator() { };
	ProbabilityEstimator(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	~ProbabilityEstimator();

	void initialize(void);

	bool runBlock(void);

	void setZScore(double z) { zScore = z; };
	double getZScore() { return zScore; };

	void setFileName(string fName) { fileName = fName; };
	string getFileName() { return fileName; };

private:

	/*Input Parameters*/
	double zScore{ 2.576 };				// For a 99% Confidence Interval
	string fileName{ "results.txt" };

	/*State variables*/
	double numberOf1{ 0 };
	double numberOf0{ 0 };
	double numberOfReceivedBits{ 0 };

};

#endif // ! PROBABILITY_ESTIMATOR_H


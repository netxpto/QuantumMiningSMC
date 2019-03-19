# ifndef PROGRAM_INCLUDE_GAUSSIAN_SOURCE_H_
# define PROGRAM_INCLUDE_GAUSSIAN_SOURCE_H_

# include "netxpto_20180118.h"
#include <random>

// Simulates a random Gaussian source
class GaussianSource : public Block {

	bool firstTime{ true };
	int aux = 0;
	bool firstPass{ true };
	double average = 0;
	double var = 1;

public:

	default_random_engine generator;

	GaussianSource() {};
	GaussianSource(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);

	void setAverage(double Average) { average = Average; }
	double const getAverage(void) { return average; }

	void setVariance(double Variance) { var = Variance; }
	double const getVariance(void) { return var; }

private:
};


#endif // !PROGRAM_INCLUDE_GAUSSIAN_SOURCE_H_

# ifndef PROGRAM_INCLUDE_ENTROPY_ESTIMATOR_H_
# define PROGRAM_INCLUDE_ENTROPY_ESTIMATOR_H_

# include "netxpto_20180418.h"
# include <vector>

// Takes a sampling of the input electrical signal
class EntropyEstimator : public Block {

public:
	void initialize(void);
	bool runBlock(void);


	EntropyEstimator() {};
	EntropyEstimator(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

private:
	int totalSymbols = 0;

	/* Order 2 */
	double probability[2] = { 0, 0 };
	int counter[2] = { 0, 0 };
};

#endif // !PROGRAM_INCLUDE_SOURCE_CODE_EFFICIENCY_H_

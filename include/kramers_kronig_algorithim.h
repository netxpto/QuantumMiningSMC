#pragma once
#pragma once
# ifndef PROGRAM_INCLUDE_KRAMERS_KRONIG_ALGORITHIM_H_
# define PROGRAM_INCLUDE_KRAMERS_KRONIG_ALGORITHIM_H_

# include "netxpto.h"
#include <random>

using namespace std;
enum algorithimType { frequency_domain, time_domain};


class KramersKronigAlgorithim : public Block {

	bool firstTime{ true };


public:

	KramersKronigAlgorithim() {};
	KramersKronigAlgorithim(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setAlgorithimType(algorithimType fType) { type = fType; };
	algorithimType const getAlgorithimType(void) { return type; };

	void setTransferFunctionLength(int transFunctionLength) { transferFunctionLength = transFunctionLength; };
	int const getTransferFunctionLength(void) { return transferFunctionLength; };


private:
	int K{ 0 };
	int L{ 0 };
	vector<t_complex> previousCopy;
	vector<t_complex> previousCopyMagnitude;
	algorithimType type { frequency_domain };
	int hilbertTransferImpulseResponseLength{ 257 };
	int transferFunctionLength{ 1024 };

};

#endif // 
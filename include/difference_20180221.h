# ifndef PROGRAM_INCLUDE_DIFFERENCE_H_
# define PROGRAM_INCLUDE_DIFFERENCE_H_

#include "netxpto_20180118.h"

// Simulates a simple difference block 
class Difference : public Block {

	bool firstTime{ true };

public:

	Difference() {};
	Difference(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);
	
private:
};

#endif

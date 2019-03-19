#pragma once
# ifndef PROGRAM_DC_FILTER_H_
# define PROGRAM_DC_FILTER_H_

# include "netxpto.h"

// Simulates a simple adding block 
class DcFilter : public Block {

	bool firstTime{ true };

public:

	DcFilter() {};
	DcFilter(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

private:
};


#endif

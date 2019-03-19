#pragma once
#pragma once
# ifndef PROGRAM_INCLUDE_DOWNSAMPLING_H_
# define PROGRAM_INCLUDE_DOWNSAMPLING_H_

# include "netxpto.h"

// Simulates a simple adding block 
class DownConvertor : public Block {

	bool firstTime{ true };

public:

	DownConvertor() {};
	DownConvertor(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

private:
};


#endif

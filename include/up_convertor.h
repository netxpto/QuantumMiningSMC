#pragma once
# ifndef PROGRAM_INCLUDE_UPSAMPLING_H_
# define PROGRAM_INCLUDE_UPSAMPLING_H_

# include "netxpto.h"

// Simulates a simple adding block 
class UpConvertor : public Block {

	bool firstTime{ true };

public:

	UpConvertor() {};
	UpConvertor(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

private:
};


#endif

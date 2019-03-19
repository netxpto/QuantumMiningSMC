#pragma once
# ifndef PROGRAM_INCLUDE_REMOVE_H_
# define PROGRAM_INCLUDE_REMOVE_H_

# include "netxpto.h"

// Simulates a simple adding block 
class Remove : public Block {

	bool firstTime{ true };

public:

	Remove() {};
	Remove(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

private:
};


#endif

# ifndef PROGRAM_INCLUDE_ADD_H_
# define PROGRAM_INCLUDE_ADD_H_

# include "netxpto_20180118.h"

// Simulates a simple adding block 
class Add : public Block {

	bool firstTime{ true };

public:

	Add() {};
	Add(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);
	
private:
};


#endif _

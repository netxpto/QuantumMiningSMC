#ifndef PROGRAM_INCLUDE_SOURCESTRING_H_
#define PROGRAM_INCLUDE_SOURCESTRING_H_

#include "netxpto_20180418.h"

// Simulates a simple adding block 
class SourceString : public Block {

	bool firstTime{ true };

public:

	SourceString() {};
	SourceString(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);
	
private:
};


#endif _

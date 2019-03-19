# ifndef PROGRAM_INCLUDE_ADD_H_
# define PROGRAM_INCLUDE_ADD_H_

# include "netxpto.h"

// Simulates a simple adding block 
class Adder : public Block {

	bool firstTime{ true };

public:

	Adder() {};
	Adder(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);
	
private:
};


#endif

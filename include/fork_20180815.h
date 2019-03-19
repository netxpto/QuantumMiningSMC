# ifndef PROGRAM_FOR_FORK_H_
# define PROGRAM_FOR_FORK_H_

# include "../include/netxpto_20180815.h"

using namespace std;

class Fork : public Block
{
public:

	// ###########################################################################################
	
	Fork(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	// ###########################################################################################

};
# endif



# ifndef PROGRAM_FOR_FORK_H_
# define PROGRAM_FOR_FORK_H_

# include <vector>
# include "netxpto.h"

using namespace std;

class Fork : public Block
{
	/* Input Parameters */

public:
	/* Methods */
	Fork() {};
	Fork(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :Block(InputSig, OutputSig) {};
	void initialize(void);
	bool runBlock(void);
	
};
# endif



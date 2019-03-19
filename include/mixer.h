/*
# ifndef PROGRAM_FOR_MIXER_H_
# define PROGRAM_FOR_MIXER_H_

# include <vector>
# include "netxpto.h"

using namespace std;
*/

/*
class Mixer : public Block
{
	//bool firstTime{ true };
	/* Input Parameters */
/*
public:
	/* Methods */
	/*Mixer() {};
	Mixer(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :Block(InputSig, OutputSig) {};
	void initialize(void);
	bool runBlock(void);


}; 
# endif*/

# ifndef PROGRAM_INCLUDE_MIXER_H_
# define PROGRAM_INCLUDE_MIXER_H_

# include "netxpto_20180118.h"

// Simulates a simple difference block 
class Mixer : public Block {

	bool firstTime{ true };

public:

	Mixer() {};
	Mixer(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

private:
};

#endif




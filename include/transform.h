#pragma once
# ifndef PROGRAM_FOR_TRANSFORM_H_
# define PROGRAM_FOR_TRANSFORM_H_

# include <vector>
# include "netxpto.h"

using namespace std;

class Transform : public Block
{
	/* Input Parameters */

public:
	/* Methods */
	Transform() {};
	Transform(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :Block(InputSig, OutputSig) {};
	void initialize(void);
	bool runBlock(void);

};
# endif



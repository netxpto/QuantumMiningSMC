#ifndef  BobBB84_H
#define BobBB84_H

# include "netxpto_20180418.h"

class bobBB84 : public Block {
	/* Input Parameters */

public:
	/*	Methods	*/
	bobBB84() {};
	bobBB84(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);
	void initialize(void);
	bool runBlock(void);

};

#endif // ! BobBB84_H


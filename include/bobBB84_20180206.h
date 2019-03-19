#ifndef bobBB84_H
#define bobBB84_H

#include "netxpto_20180118.h"


class bobBB84 : public Block {

	/*Input Parameters*/
	

public:

	bobBB84() {};
	bobBB84(vector<Signal *> &inputSignals, vector<Signal *> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);

	bool runBlock(void);


};

#endif // !bobBB84_H


#ifndef BINARY_MAPPER_H
#define BINARY_MAPPER_H
	
#include "netxpto.h"

class BinaryMapper : public Block {
	/* State Variables */

public:
	/* Input Parameters */
	int high{ 45 };
	int low{ 0 };

	/*Methods*/
	BinaryMapper() {};
	BinaryMapper(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);

	bool runBlock(void);

};



#endif // !BINARY_MAPPER_H


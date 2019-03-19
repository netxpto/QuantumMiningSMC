#ifndef DEMUX_1_2_H
#define DEMUX_1_2_h

#include "netxpto.h"

class Demux_1_2 : public Block {

public:
	/*input parameters*/

	/*Methods*/
	Demux_1_2() {};
	Demux_1_2(vector <Signal*> &inputSignals, vector <Signal*> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);

	bool runBlock(void);


};

#endif

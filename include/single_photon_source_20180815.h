# ifndef SINGLE_PHOTON_SOURCE_H_
# define SINGLE_PHOTON_SOURCE_H_

#include "../include/netxpto_20180815.h"

class SinglePhotonSource : public Block {

public:

	/*	Methods	*/
	SinglePhotonSource(initializer_list<Signal *> inputSignals, initializer_list<Signal *> outputSignals) : Block(inputSignals, outputSignals) {};
	SinglePhotonSource(vector<Signal *> &inputSignals, vector<Signal *> &outputSignals) : Block(inputSignals, outputSignals){};

	void initialize(void);
	bool runBlock(void);

};

#endif

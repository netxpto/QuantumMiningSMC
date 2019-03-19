#ifndef POLARIZE_H_
#define POLARIZER_H_

#include "../include/netxpto_20180815.h"

//enum functionalMode {Sender, Receiver};

class Polarizer : public Block{
	
public:

	//Methods
	Polarizer (initializer_list<Signal*> InputSig, initializer_list<Signal*> OutputSig) : Block(InputSig, OutputSig) {};
	Polarizer (vector <Signal*> &inputSignals, vector <Signal*> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);

	bool runBlock(void);

	
};
#endif // !POLARIZE_H_


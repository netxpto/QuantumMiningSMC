#ifndef POLARIZE_H_
#define POLARIZER_H_

# include <algorithm>  // min()
# include <math.h>     // cos(), sin()
#include <array>
#include "netxpto_20180118.h"

enum functionalMode {Sender, Receiver};

class Polarizer : public Block{
	
	
public:
	// Input parameters


	//Methods
	Polarizer (vector <Signal*> &inputSignals, vector <Signal*> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);

	bool runBlock(void);

	
};
#endif // !POLARIZE_H_


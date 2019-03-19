#ifndef POLARIZE_H_
#define POLARIZER_H_

# include <algorithm>  // min()
# include <math.h>     // cos(), sin()
#include <array>
#include "netxpto_20180418.h"


class PolarizationRotator : public Block{
	
	
public:
	// Input parameters
	double tetha{ 0.0 };
	double phi{ 0.0 };

	//Methods
	PolarizationRotator(vector <Signal*> &inputSignals, vector <Signal*> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);

	bool runBlock(void);

	
};
#endif // !POLARIZE_H_


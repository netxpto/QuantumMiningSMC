#ifndef ELECTRONIC_POLARIZATION_CONTROLLER_H_
#define ELECTRONIC_POLARIZATION_CONTROLLER_H_

# include <algorithm>  // min()
# include <math.h>     // cos(), sin()
#include <array>
#include "../include/netxpto_20180815.h"



class ElectronicPolarizationController : public Block{
	
	
public:
	//##############################################################################################################

	ElectronicPolarizationController(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};
	ElectronicPolarizationController(vector <Signal*> &inputSignals, vector <Signal*> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);
	bool runBlock(void);
	//#############################################################################################################

private:
	// Input parameters
	
	//System state variables
	t_complex rotMatrix[2][2]{ {1,0},{0,1} };
	t_complex input1{ };
	t_complex input2{ };
	t_complex input3{ };
	t_complex input4{ };
	
};
#endif // !ELECTRONIC_POLARIZATION_CONTROLLER_H_


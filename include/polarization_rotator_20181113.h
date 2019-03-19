#ifndef POLARIZATION_ROTATOR_H_
#define POLARIZATION_ROTATOR_H_

# include <algorithm>  // min()
# include <math.h>     // cos(), sin()
#include <array>
#include "../include/netxpto_20180815.h"



class PolarizationRotator : public Block{
	
	
public:
	//##############################################################################################################

	PolarizationRotator(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};
	PolarizationRotator(vector <Signal*> &inputSignals, vector <Signal*> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);
	bool runBlock(void);
	//#############################################################################################################

private:
	// Input parameters
	double theta{ 0.0 };	// Radians 
	double phi{ 0.0 };		// Radians
	int n{ 0 };
	
	//System state variables
	double rotation[3][3] = { {cos(phi)*cos(theta), -cos(phi)*sin(theta), sin(phi)},
								{cos(phi)*sin(theta) + pow(sin(phi),2)*cos(theta), cos(phi)*cos(theta) - pow(sin(phi),2)*sin(theta), -sin(phi)*cos(phi)},
								{sin(phi)*sin(theta) - sin(phi)*cos(phi)*cos(theta), sin(phi)*cos(theta) + cos(phi)*sin(phi)*sin(theta), pow(cos(phi),2)}
	};
};
#endif // !POLARIZE_H_


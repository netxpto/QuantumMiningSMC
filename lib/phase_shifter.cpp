#include <algorithm>
#include <complex>
#include <cmath> 

#include "netxpto.h"
#include "phase_shifter.h"


void PhaseShifter::initialize(void) {

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}


bool PhaseShifter::runBlock(void) {
	int ready = inputSignals[0]->ready();

	int space1 = outputSignals[0]->space();
	int space2 = outputSignals[0]->space();
	int space = min(space1, space2);

	int process = min(ready, space);

	if (process == 0) return false;

	//t_complex imaginary = (0, 1);

	for (int i = 0; i < process; i++) 
	{

		t_real input; // Input cosine singal
		t_real out;	  //  Output sine signal	

		inputSignals[0]->bufferGet(&input);

		t_real inputA;
		inputA = acos( input/(0.5*sqrt(rfAmplitude)) );
		
		t_real inputB;
		inputB = PI/2 + inputA; // Add PI/2 phase shift and adjust sign according to the exponential (e^{sign*2*PI*f*T}).

		out = 0.5*sqrt(rfAmplitude)*sin(inputA);    // Here sign is negative due to the negative exponential

		outputSignals[0]->bufferPut(out);


	}
	return true;
}
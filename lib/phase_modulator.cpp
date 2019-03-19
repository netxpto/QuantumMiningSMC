#include <algorithm>
#include <complex>
#include <iostream>
#include <fstream>

#include "netxpto.h"
#include "phase_modulator.h"


void PhaseModulator::initialize(void){

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[1]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[1]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[1]->getFirstValueToBeSaved());
	 
	outputSignals[0]->setCentralWavelength(inputSignals[1]->getCentralWavelength());
	outputSignals[0]->setCentralFrequency(inputSignals[1]->getCentralWavelength());

}


bool PhaseModulator::runBlock(void){

	double phase;
	
	vector<int> const1 = { 1,1 };
	vector<int> const2 = { 0,0 };
	vector<int> const3 = { 1,0 };
	vector<int> const4 = { 0,1 };
	
	int samplesPerSymbol = static_cast<int> (inputSignals[1]->getSamplesPerSymbol());

	if (firstPass)
	{
		firstPass = false;
		delay.resize(bitsPerSymbol);
	}
	
	int ready1 = inputSignals[0]->ready(); // Number of values in Binary String
	int ready2 = inputSignals[1]->ready(); // Number of samples in Optical Signal

	int ready11 = static_cast<int> ( floor( ready1 / bitsPerSymbol ) ); // Number of Symbols in Binary String
	int ready22 = static_cast<int> ( floor( ready2 / samplesPerSymbol ) ); // Number of Symbols in Optical Signal

	int ready = min(ready11, ready22); // In number of symbols
	int space = static_cast<int> ((outputSignals[0]->space())/samplesPerSymbol); // In number of symbols

	int process = min(ready, space);

	if (process == 0) return false;
	
	complex<t_real> imaginary(0, 1);
	
	for (int i = 0; i < process; i++) {

		for (int ctrB = 0; ctrB < bitsPerSymbol; ctrB++)
		{
			t_binary inB;
			inputSignals[0]->bufferGet(&inB);
			delay[ctrB] = inB;
		}
		if (equal(delay.begin(), delay.end(), const1.begin())) phase = PI/4;
		if (equal(delay.begin(), delay.end(), const2.begin())) phase = PI/4-PI/2;
		if (equal(delay.begin(), delay.end(), const3.begin())) phase = PI/4-PI;
		if (equal(delay.begin(), delay.end(), const4.begin())) phase = PI/4-3*PI/2;
		for (int ctrO = 0; ctrO < samplesPerSymbol-1; ctrO++)
		{
			t_complex inO;
			t_complex out;
			inputSignals[1]->bufferGet(&inO);
			t_complex caster = exp(imaginary*phase);
			out = inO * caster;
			outputSignals[0]->bufferPut(out);
		}
		
	}
	return true;
}

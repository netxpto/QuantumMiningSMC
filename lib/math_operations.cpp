
#include <algorithm>
#include <complex>
#include <fstream>

#include "netxpto.h"
#include "math_operations.h"



void Math::initialize(void) {

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());


}


bool Math::runBlock(void) {

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process == 0) return false;

	signal_value_type sTypeIn = inputSignals[0]->getValueType();
	signal_value_type sTypeOut = outputSignals[0]->getValueType();


	if ( sTypeOut != sTypeIn)
	{
		cout << "ERROR: math_operations.cpp (signal type mismatch!)" << "\n";
		return false;
	}

	MathOperationType opType = getMathOpertationType();

	switch (opType)
	{

	case Sqrt:
	{
		t_complex in;
		for (int k = 0; k < process; k++)
		{
			inputSignals[0]->bufferGet(&in);
			outputSignals[0]->bufferPut(sqrt(in));
		}
		break;
	}

	case NaturalLog:
	{
		t_complex in;
		for (int k = 0; k < process; k++)
		{
			inputSignals[0]->bufferGet(&in);
			outputSignals[0]->bufferPut(log(in));
		}
		break;
	}

	case Exponential:
	{
		t_complex in;
		for (int k = 0; k < process; k++) 
		{
			inputSignals[0]->bufferGet(&in);
			outputSignals[0]->bufferPut(exp(in));
		}
		break;
	}

	}
	return true;
}



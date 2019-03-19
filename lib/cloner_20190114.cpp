# include "cloner_20190114.h"

# include <algorithm>
# include <complex>
# include <iostream>
# include <fstream>
# include <random>
# include <numeric>

using namespace std;


void Cloner::initialize(void) {
	for (size_t i = 0; i < numberOfOutputSignals; i++)
	{
		outputSignals[i]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
		outputSignals[i]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
		outputSignals[i]->writeHeader();
	}
}


bool Cloner::runBlock(void) {

	signal_value_type sTypeIn = inputSignals[0]->getValueType();

	int ready = inputSignals[0]->ready();
	int process = ready;
	for (size_t i = 0; i< numberOfOutputSignals; i++)
	{
		int space = outputSignals[i]->space();
		process = min(ready, min(space, process));

		signal_value_type sTypeOut = outputSignals[i]->getValueType();
		if (sTypeIn != sTypeOut)
		{
			cout << "ERRO: cloner_20190114.cpp (signal type mismatch!)" << "\n";
			return false;
		}
	}

	if (process == 0) return false;

	switch (sTypeIn)
	{
	case RealValue:
	{
		t_real in;
		t_real out;
		for (int i = 0; i < process; i++)
		{
			inputSignals[0]->bufferGet(&in);
			out = in;

			for (size_t j = 0; j < numberOfOutputSignals; j++) outputSignals[j]->bufferPut(out);
		}

		break;
	}
	case BinaryValue:
	{
		t_binary in;
		t_binary out;
		for (int i = 0; i < process; i++)
		{
			inputSignals[0]->bufferGet(&in);
			out = in;

			for (size_t j = 0; j < numberOfOutputSignals; j++) outputSignals[j]->bufferPut(out);
		}
	
		break;
	}
	case ComplexValueXY:
	{
		t_complex_xy in;
		t_complex_xy out;
		for (int i = 0; i < process; i++)
		{
			inputSignals[0]->bufferGet(&in);
			out = in;

			for (size_t j = 0; j < numberOfOutputSignals; j++) outputSignals[j]->bufferPut(out);
		}
	
		break;
	}
	case ComplexValue:
	{
		t_complex in;
		t_complex out;
		for (int i = 0; i < process; i++)
		{
			inputSignals[0]->bufferGet(&in);
			out = in;

			for (size_t j = 0; j < numberOfOutputSignals; j++)
			{
				outputSignals[j]->bufferPut(out);
			}
		}
	
		break;
	}
	}
	
	return true;
}

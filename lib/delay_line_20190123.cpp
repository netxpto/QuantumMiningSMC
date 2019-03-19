#include <algorithm>
#include <complex>
#include <iostream>

#include "netxpto.h"
#include "delay_line_20190123.h"



void DelayLine::initialize(void){

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
}


bool DelayLine::runBlock(void){

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();


	t_complex in;


	if (firstTime)
	{
		if (ready > aux )
		{
			firstTime = false;
			for (size_t i = 0; i < delay; i++)
			{
				inputSignals[0]->bufferGet(&in);
				ready--;
			}
		}
		else
		{
			aux-= ready;
			for (size_t i = 0; i < ready; i++)
			{
				inputSignals[0]->bufferGet(&in);
				ready--;
			}
		}
	}

	int process = min(ready, space);
	if (process == 0)
	{
		/*if (firstEnd && space > delay)
		{
			firstEnd = false;
			in = polar(0.0, 0.0);
			for (size_t i = 0; i < delay; i++) outputSignals[0]->bufferPut(in);
		}*/
		return false;
	}
	for (int i = 0; i < process; i++) 
	{
		inputSignals[0]->bufferGet(&in);
		outputSignals[0]->bufferPut(in);
	}

	return true;
}
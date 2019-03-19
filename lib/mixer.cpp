/*
#include "netxpto.h"

#include "mixer.h"

using namespace std;

void Mixer::initialize(void)
{
		double samplingPeriod1 = inputSignals[0]->samplingPeriod;			// Signal arrives from pulse_shapper output 
	double symbolPeriod1  = inputSignals[0]->symbolPeriod;

	double samplingPeriod2 = inputSignals[1]->samplingPeriod;			// Signal arrives from RF_Osclillator output
	double symbolPeriod2  = inputSignals[1]->symbolPeriod;

	if (samplingPeriod1 != samplingPeriod2) { cout << "ERROR: Mixer.cpp - samplingPeriod\n"; };
	
	outputSignals[0]->setSamplingPeriod(samplingPeriod1);

	if (symbolPeriod1 == 1 && symbolPeriod2 == 1) { cout << "ERROR: Mixer.cpp - symbolPeriod\n"; };

	if (symbolPeriod1 == 1) {
		outputSignals[0]->setSymbolPeriod(symbolPeriod2);
	} 
	else {
		outputSignals[0]->setSymbolPeriod(symbolPeriod1);
	};
}


bool Mixer::runBlock(void)
{

	int ready1 = inputSignals[0]->ready();
	int ready2 = inputSignals[1]->ready();
	int ready  = min(ready1, ready2);

	int space = outputSignals[0]->space();

	int process = min(space, ready);
	if (process == 0) return false;

	
	for (int i = 0; i < process; i++)
	{
		t_real inRealValue1;
		t_real inRealValue2;
		t_real outRealValue;
		inputSignals[0]->bufferGet(&inRealValue1);
		inputSignals[1]->bufferGet(&inRealValue2);
		outRealValue = inRealValue1 * inRealValue2;
		outputSignals[0]->bufferPut(outRealValue);
		
	}

	return true;

}*/



#include <algorithm>
#include <complex>
#include <fstream>

#include "mixer.h"



void Mixer::initialize(void) {

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());


}


bool Mixer::runBlock(void) {

	int ready1 = inputSignals[0]->ready();
	int ready2 = inputSignals[1]->ready();
	int ready = min(ready1, ready2);

	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process == 0) return false;

	signal_value_type sType1 = inputSignals[0]->getValueType();
	signal_value_type sType2 = inputSignals[1]->getValueType();
	signal_value_type sTypeOut = outputSignals[0]->getValueType();

	if (sType1 != sType2 || sTypeOut != sType1)
	{
		cout << "ERROR: mixer.cpp (signal type mismatch!)" << "\n";
		return false;
	}

	switch (sType1)
	{

	case RealValue:
	{
		t_real inRealValue1;
		t_real inRealValue2;
		t_real outRealValue;

		for (int k = 0; k < process; k++)
		{
			inputSignals[0]->bufferGet(&inRealValue1);
			inputSignals[1]->bufferGet(&inRealValue2);
			outRealValue = inRealValue1*inRealValue2;
			outputSignals[0]->bufferPut(outRealValue);
		}
		break;
	}

	case ComplexValue:
	{
		t_complex inComplexValue1;
		t_complex inComplexValue2;
		t_complex outComplexValue;

		for (int k = 0; k < process; k++)
		{
			inputSignals[0]->bufferGet(&inComplexValue1);
			inputSignals[1]->bufferGet(&inComplexValue2);
			outComplexValue = inComplexValue1*inComplexValue2;
			outputSignals[0]->bufferPut(outComplexValue);
		}
		break;
	}

	case ComplexValueXY:
	{
		t_complex_xy inComplexValueXY1;
		t_complex_xy inComplexValueXY2;
		t_complex_xy outComplexValueXY;

		for (int k = 0; k < process; k++) 
		{
			inputSignals[0]->bufferGet(&inComplexValueXY1);
			inputSignals[1]->bufferGet(&inComplexValueXY2);
			outComplexValueXY = {
				inComplexValueXY1.x * inComplexValueXY2.x,
				inComplexValueXY1.y * inComplexValueXY2.y};
			outputSignals[0]->bufferPut(outComplexValueXY);
		}
		break;
	}

	}
	return true;
}



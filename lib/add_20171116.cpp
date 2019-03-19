#include <algorithm>
#include <complex>
#include <fstream>

#include "netxpto_20180418.h"
#include "add_20171116.h"


void Add::initialize(void){

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}


bool Add::runBlock(void){

	int ready1 = inputSignals[0]->ready();
	int ready2 = inputSignals[1]->ready();
	int ready = min(ready1, ready2);

	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process == 0) return false;

	signal_value_type sType1 = inputSignals[0]->getValueType();
	signal_value_type sType2 = inputSignals[1]->getValueType();
	signal_value_type sTypeOut = outputSignals[0]->getValueType();

	if (sType1!=sType2 || sTypeOut!=sType1)
	{
		cout << "ERRO: add.cpp (signal type mismatch!)" << "\n";
		return false;
	}

	switch (sType1)
	{

	case RealValue:
	{
		for (int k = 0; k < process; k++) {
			t_real inRealValue1;
			inputSignals[0]->bufferGet(&inRealValue1);
			t_real inRealValue2;
			inputSignals[1]->bufferGet(&inRealValue2);
			t_real outRealValue = inRealValue1 + inRealValue2;
			outputSignals[0]->bufferPut(outRealValue);
		}
		break;
	}
	
	case ComplexValue:
	{
		for (int k = 0; k < process; k++) {
			t_complex inComplexValue1;
			inputSignals[0]->bufferGet(&inComplexValue1);
			t_complex inComplexValue2;
			inputSignals[1]->bufferGet(&inComplexValue2);
			t_complex outComplexValue = inComplexValue1 + inComplexValue2;
			outputSignals[0]->bufferPut(outComplexValue);
		}
		break;
	}
	
	case ComplexValueXY:
	{
		for (int k = 0; k < process; k++) {
			t_complex_xy inComplexValueXY1;
			inputSignals[0]->bufferGet(&inComplexValueXY1);
			t_complex_xy inComplexValueXY2;
			inputSignals[1]->bufferGet(&inComplexValueXY2);
			t_complex_xy outComplexValueXY = { inComplexValueXY1.x + inComplexValueXY2.x, inComplexValueXY1.y + inComplexValueXY2.y };
			outputSignals[0]->bufferPut(outComplexValueXY);
		}
		break;
	}
	/*case ComplexValueXY:
	{
						   for (int k = 0; k < process; k++) {

							   t_complex_xy in;
							   inputSignals[0]->bufferGet(&in);
							   t_complex_xy noise;
							   inputSignals[1]->bufferGet(&noise);

							   t_complex_xy out = { in.x + noise.x, in.y + noise.y };

							   outputSignals[0]->bufferPut(out);
						   }
						 break;
	}*/
	}
	return true;
}

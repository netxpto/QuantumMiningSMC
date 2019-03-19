# include "netxpto_20180418.h"
# include "ideal_amplifier_20180118.h"
# include <algorithm> // min

using namespace std;

void IdealAmplifier::initialize(void){


	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}



bool IdealAmplifier::runBlock(void) {

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process == 0) return false;
	
	signal_value_type sTypeIn = inputSignals[0]->getValueType();
	signal_value_type sTypeOut= outputSignals[0]->getValueType();

	if (sTypeIn!=sTypeOut) {
        	cout << "ERRO: ideal_amplifier.cpp (signal type mismatch!)" << "\n";
        	return false;
    	}

	switch (sTypeIn) {
	case RealValue:
	{
					  t_real inReal;
					  for (int k = 0; k < process; k++) {
						  inputSignals[0]->bufferGet(&inReal);
						  outputSignals[0]->bufferPut((t_real)gain*inReal);
					  }
					  break;
	}
	case ComplexValue:
	{
						 t_complex inComplexValue;
						 for (int k = 0; k < process; k++) {
							 inputSignals[0]->bufferGet(&inComplexValue);
							 outputSignals[0]->bufferPut((t_complex)gain*inComplexValue);
						 }
						 break;
	}
	case ComplexValueXY:
	{
						   t_complex_xy inComplexValueXY;
						   for (int k = 0; k < process; k++) {
							   inputSignals[0]->bufferGet(&inComplexValueXY);
							   t_complex_xy outComplexValueXY = { gain*inComplexValueXY.x, gain*inComplexValueXY.y };
							   outputSignals[0]->bufferPut(outComplexValueXY);
							   break;
						   }
						   break;
	}
	}
	return true;
}

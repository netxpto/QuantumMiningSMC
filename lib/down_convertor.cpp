#include <algorithm>
#include <complex>
#include <fstream>

#include "netxpto.h"
# include "down_convertor.h"


void DownConvertor::initialize(void) {

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}


bool DownConvertor::runBlock(void)
{

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
		cout << "ERRO: add.cpp (signal type mismatch!)" << "\n";
		return false;
	}

	switch (sType1)
	{

	case RealValue:
	{
		for (int k = 0; k < process; k++)
		{
			t_real inRealValue1;
			t_real inRealValue2;
			t_real out;

			inputSignals[0]->bufferGet(&inRealValue1);
			inputSignals[1]->bufferGet(&inRealValue2);

			out = inRealValue1 * inRealValue2;

			outputSignals[0]->bufferPut(t_real(out));

		}
		break;
	}

	case ComplexValue:
	{
		for (int k = 0; k < process; k++)
		{
			t_complex inComplexValue1;
			t_complex inComplexValue2;
			t_complex out;

			inputSignals[0]->bufferGet(&inComplexValue1);
			inputSignals[1]->bufferGet(&inComplexValue2);

			out = inComplexValue1 * inComplexValue2;

			outputSignals[0]->bufferPut(t_complex(out));

		}
		break;
	}

	}

	return true;
}

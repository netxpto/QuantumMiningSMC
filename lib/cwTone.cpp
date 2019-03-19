#include <algorithm>
#include <complex>
#include <fstream>

#include "netxpto.h"
#include "cwTone.h"
#include "fft_20180208.h"



void CwTone::initialize(void) {

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	outputSignals[1]->setSymbolPeriod(inputSignals[1]->getSymbolPeriod());
	outputSignals[1]->setSamplingPeriod(inputSignals[1]->getSamplingPeriod());
	outputSignals[1]->setFirstValueToBeSaved(inputSignals[1]->getFirstValueToBeSaved());
}


bool CwTone::runBlock(void) {

	int ready1 = inputSignals[0]->ready();
	int ready2 = inputSignals[1]->ready();
	int ready = min(ready1, ready2);

	int space1 = outputSignals[0]->space();
	int space2 = outputSignals[1]->space();
	int space = min(space1, space2);

	int process = min(ready, space);
	if (process == 0) return false;

	double dt = outputSignals[0]->getSamplingPeriod();

	for (int k = 0; k < process; k++)
		{
			t_real inRealValue1;
			t_real inRealValue2;
			t_real outRealValue1;
			t_real outRealValue2;

			t_real cwTone0;				// For cos(pi*B*t)
			t_real cwTone90;			// For sin(pi*B*t)

			cwPhase = cwPhase + 2 * PI * cwFrequency * dt;
			cwTone0 =  cwAmplitude*cos(cwPhase);
			cwTone90 = cwAmplitude*sin(cwPhase);

			inputSignals[0]->bufferGet(&inRealValue1);
			inputSignals[1]->bufferGet(&inRealValue2);
			outRealValue1 = inRealValue1 + cwTone0;
			outRealValue2 = inRealValue2 + cwTone90;
			outputSignals[0]->bufferPut(outRealValue1);
			outputSignals[1]->bufferPut(outRealValue2);
		}
	return true;
}



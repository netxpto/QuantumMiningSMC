# include <algorithm>	// std::min
# include <complex.h>   // sta

# include "netxpto.h"
# include "pin.h"


using namespace std;

void Pin::initialize(void) {

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}

bool Pin::runBlock(void) {

	int ready = inputSignals[0]->ready();
	
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process == 0) return false;

	t_complex inSig;
	for (int i = 0; i < process; i++) {

		inputSignals[0]->bufferGet(&inSig);

		outputSignals[0]->bufferPut(2 * responsivity * abs(inSig) * abs(inSig));

	}

	return true;
}

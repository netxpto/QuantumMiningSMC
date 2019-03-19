#include <algorithm>
#include <complex>

#include "netxpto.h"
#include "discarder.h"


discarder::discarder(vector <Signal *> &InputSig, vector <Signal *> &OutputSig){

	numberOfInputSignals = InputSig.size();
	numberOfOutputSignals = OutputSig.size();

	inputSignals = InputSig;
	outputSignals = OutputSig;

	outputSignals[0]->symbolPeriod = inputSignals[0]->symbolPeriod;;
	outputSignals[0]->samplingPeriod = inputSignals[0]->samplingPeriod;


}


bool discarder::runBlock(void){
	int ready = inputSignals[0]->ready();
	int space0 = outputSignals[0]->space();
	int process = min(ready, space0);
	t_binary in;
	t_binary out;

	int auxint=0;
	if (process == 0) return false;

	for (int i = 0; i < ready; i++) {

		auxint++;
		inputSignals[0]->bufferGet(&in);
		if (auxint == 1)
		{
			// do nothing
		}
		else if (auxint == 2)
		{
			out = in;
			outputSignals[0]->bufferPut(out);
		}

	}

	return true;
}

/*
void bit_decider::setBitPeriod(double bPeriod){
bitPeriod = bPeriod;
outputSignals[0]->symbolPeriod = bitPeriod;
outputSignals[0]->samplingPeriod = outputSignals[0]->symbolPeriod;
};


class ContinuousToDiscreteTime : public Block {
public:
ContinuousToDiscreteTime(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);
bool runBlock(void);

int numberOfSamplesPerSymbol{ 8 };
int index{ 0 };

void setNumberOfSamplesPerSymbol(int nSamplesPerSymbol){ numberOfSamplesPerSymbol = nSamplesPerSymbol; outputSignals[0]->samplingPeriod = (inputSignals[0]->samplingPeriod) / numberOfSamplesPerSymbol; };
};
*/
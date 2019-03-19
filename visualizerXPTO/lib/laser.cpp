# include "netplus.h"
# include "laser.h"

using namespace std;

Laser::Laser(vector<Signal*> &InputSig, vector<Signal*> &OutputSig) {

  numberOfInputSignals = InputSig.size();
  numberOfOutputSignals = OutputSig.size();

  inputSignals = InputSig;
  outputSignals = OutputSig;

  outputSignals[0]->setSamplingPeriod(samplingPeriod);
  outputSignals[0]->setCentralWavelength(centralWavelength)

}

bool Laser::runBlock(void) {

	int space = outputSignals[0]->space();

	if (space == 0) return false;

	for (int k = 0; k < space; k++) outputSignals[0]->bufferPut(polar(1,initialPhase));

	return true;
}

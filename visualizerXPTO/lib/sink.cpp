# include "netplus.h"
# include "sink.h"

# include <algorithm>   // std::min
# include <string>		// compare

using namespace std;

Sink::Sink(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) {

  numberOfInputSignals = InputSig.size();
  numberOfOutputSignals = OutputSig.size();

  inputSignals = InputSig;
}

bool Sink::runBlock(void)
{

	int ready = inputSignals[0]->ready();

	int process;
	if (numberOfSamples >= 0) {
		process = std::min((long int)ready, numberOfSamples);
	}
	else {
		process = ready;
	}
	 
	if ((process == 0) || (numberOfSamples==0)) return false;

/*	if ((inputSignals[0])->getType().compare("TimeDiscreteAmplitudeDiscreteReal"))
		for (int i = 0; i<process; i++) static_cast<TimeDiscreteAmplitudeDiscreteReal *>(inputSignals[0])->bufferGet();

	if ((inputSignals[0])->getType().compare("Binary"))
		for (int i = 0; i<process; i++) static_cast<Binary *>(inputSignals[0])->bufferGet();

	if ((inputSignals[0])->getType().compare("TimeDiscreteAmplitudeContinuousReal"))
		for (int i = 0; i<process; i++) static_cast<TimeDiscreteAmplitudeContinuousReal *>(inputSignals[0])->bufferGet();

	if ((inputSignals[0])->getType().compare("TimeDiscreteAmplitudeContinuousComplex"))
	for (int i = 0; i<process; i++) static_cast<TimeDiscreteAmplitudeContinuousComplex *>(inputSignals[0])->bufferGet();

	if ((inputSignals[0])->getType().compare("TimeContinuousAmplitudeDiscreteReal"))
		for (int i = 0; i<process; i++) static_cast<TimeContinuousAmplitudeDiscreteReal *>(inputSignals[0])->bufferGet();

	if ((inputSignals[0])->getType().compare("TimeContinuousAmplitudeContinuousComplex"))
		for (int i = 0; i<process; i++) static_cast<TimeContinuousAmplitudeContinuousComplex *>(inputSignals[0])->bufferGet();*/


	for (int i = 0; i<process; i++) (inputSignals[0])->bufferGet();

	numberOfSamples = numberOfSamples - process;
	if (displayNumberOfSamples) cout << numberOfSamples << "\n";

	return true;
}

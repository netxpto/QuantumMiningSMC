# include "sink_20180620.h"

using namespace std;

Sink::Sink(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) {

  numberOfInputSignals = (int) InputSig.size();
  numberOfOutputSignals = (int) OutputSig.size();

  inputSignals = InputSig;
  output_file.open(asciiFilePath,ios::binary);

  if (!output_file.is_open()) { cerr << "ERROR: Could not open file " << this->asciiFilePath << endl; exit(1); }
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

	if ((process == 0) || (numberOfSamples == 0)) return false;

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

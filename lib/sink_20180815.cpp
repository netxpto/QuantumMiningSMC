# include "../include/sink_20180815.h"

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
	 
	if (process == 0) return false;

	for (int i = 0; i < process; i++) {
		inputSignals[0]->bufferGet();
		++processedSamples;
	}

	if (numberOfSamples >= 0) numberOfSamples -= process;

	if (displayNumberOfSamples) {
		cout << "Samples to be processed now: " << process << "\n";
		cout << "Samples to be processed later: " << numberOfSamples << "\n";
		cout << "Samples already processed: " << processedSamples << "\n";
	}

	return true;
}

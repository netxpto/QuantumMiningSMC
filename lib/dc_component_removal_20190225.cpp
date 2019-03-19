# include "dc_component_removal_20190225.h"

# include <complex>
# include "../algorithms/fft/fft_20180208.h"

using namespace std;


void DCComponentRemoval::initialize(void) {

	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());

	int samplesPerSymbol = inputSignals[0]->getSamplesPerSymbol();
	int bufferLength = outputSignals[0]->getBufferLength();
	

	int hcf = samplesPerSymbol;
	int temp = bufferLength;

	while (hcf != temp)
	{
		if (hcf > temp)
			hcf -= temp;
		else
			temp -= hcf;
	}

	blockSize = (samplesPerSymbol * bufferLength) / hcf;

	delayLine.resize(blockSize);
	delayLinePrevious.resize(blockSize);
	finiteImpulseResponse.resize(2 * blockSize, 0);
	for (int i = 0; i < blockSize; i++) finiteImpulseResponse[i] = 1.0 / (double)blockSize;
	fft(finiteImpulseResponse, 1, 1);
}


bool DCComponentRemoval::runBlock(void) {

	t_complex in;
	t_complex out;

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	if (count != 0)
	{
		int process = outputSignals[0]->space() - outputSignals[0]->getInPosition();
		outputSignals[0]->setOutPosition(outputSignals[0]->getInPosition());
		for (int j = 0; j < process; j++)
		{
			out = outVector[0];
			outVector.erase(outVector.begin(), outVector.begin() + 1);
			outputSignals[0]->bufferPut(out);
			count--;
			if (count == 0) break;
		}
		if (count != 0) return false;
	}

	space = outputSignals[0]->space();

	int process = min(ready, space);
	if (process == 0) return false;
	
	for (int i = 0; i < process; i++)
	{
		inputSignals[0]->bufferGet(&in);
		delayLine[aux] = in;
		
		aux++;
		if (aux == blockSize)
		{
			aux = 0;

			vector<t_complex> inVector;
			inVector.reserve(delayLine.size() + delayLinePrevious.size());
			inVector.insert(inVector.end(), delayLinePrevious.begin(), delayLinePrevious.end());
			inVector.insert(inVector.end(), delayLine.begin(), delayLine.end());
			
			fft(inVector, 1, 1);

			filtered.resize(inVector.size());
			for (size_t j = 0; j < inVector.size(); j++) filtered[j] = inVector[j] * finiteImpulseResponse[j];
			fft(filtered, -1, 1);
			filtered.erase(filtered.begin(), filtered.begin() + blockSize);
			outVector.resize(filtered.size());
			for (size_t j = 0; j < filtered.size(); j++) outVector[j] = delayLine[j] - filtered[j];
			
			count = filtered.size();

			int process = outputSignals[0]->space() - outputSignals[0]->getInPosition();
			outputSignals[0]->setOutPosition(0);
			for (int j = 0; j < process; j++)
			{
				out = outVector[0];
				outVector.erase(outVector.begin(), outVector.begin() + 1);
				outputSignals[0]->bufferPut(out);
				count--;
				if (count == 0) break;
			}
			delayLinePrevious = delayLine;
		}
	}



	return true;
}

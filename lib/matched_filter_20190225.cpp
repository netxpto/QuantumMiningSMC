# include "matched_filter_20190225.h"

using namespace std;

void rootRaisedCosine(vector<t_real> &impulseResponse, int impulseResponseLength, double rollOffFactor, double samplingPeriod, double symbolPeriod, bool passiveFilterMode);

void MatchedFilter::initialize(void) {

	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());

	int samplesPerSymbol = inputSignals[0]->getSamplesPerSymbol();

	blockSize = samplesPerSymbol * numberOfTaps;
	delayLine.resize(blockSize);
	delayLinePrevious.resize(blockSize);
	impulseResponse.resize(blockSize);
	filter.resize(2 * blockSize);
	
	double samplingPeriod = inputSignals[0]->getSamplingPeriod();
	double symbolPeriod = inputSignals[0]->getSymbolPeriod();
	
	rootRaisedCosine(impulseResponse, blockSize, rollOffFactor, samplingPeriod, symbolPeriod, true);
	
	double sum = 0;
	for (size_t i = 0; i < impulseResponse.size(); i++) sum += impulseResponse[i];
	for (size_t i = 0; i < impulseResponse.size(); i++) impulseResponse[i] = impulseResponse[i] / sum;
	for (size_t i = 0; i < impulseResponse.size(); i++) filter[i + impulseResponse.size() / 2] = impulseResponse[i];
	fft(filter);
	for (size_t i = 0; i < filter.size(); i++) filter[i] = filter[i] * sqrt(blockSize * 2);
}


bool MatchedFilter::runBlock(void) {

	t_complex in;
	t_complex out;

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();
	
	if (count != 0)
	{
		for (int j = 0; j < space; j++)
		{
			out = outVector[0];
			outVector.erase(outVector.begin(), outVector.begin() + 1);
			outputSignals[0]->bufferPut(out);
			count--;
			if (count == 0) break;
		}
		if (count != 0) return false;
	}


	int process = min(ready, space);


	if (process == 0) return false;
	
	for (int i = 0; i < ready; i++)
	{
		inputSignals[0]->bufferGet(&in);
		delayLine[aux] = in;
		
		aux++;
		if (aux == blockSize)
		{
			runCounter++;
			aux = 0;

			vector<t_complex> inVector;
			inVector.reserve(delayLine.size() + delayLinePrevious.size());
			inVector.insert(inVector.end(), delayLinePrevious.begin(), delayLinePrevious.end());
			inVector.insert(inVector.end(), delayLine.begin(), delayLine.end());

			delayLinePrevious = delayLine;
			fft(inVector);

			filtered.resize(inVector.size());
			for (size_t j = 0; j < inVector.size(); j++) filtered[j] = inVector[j] * filter[j];
			ifft(filtered);

			outVector.resize(inVector.size());

			outVector = fftshift(filtered);
			if (runCounter == 1) count = 0;
			else if (runCounter == 2)
			{
				outVector.erase(outVector.begin() + outVector.size() - blockSize / 2 - 1, outVector.begin() + outVector.size());
				count = outVector.size();
				int process = min(outputSignals[0]->space(), count);
				for (int j = 0; j < process; j++)
				{
					out = outVector[0];
					outVector.erase(outVector.begin(), outVector.begin() + 1);
					outputSignals[0]->bufferPut(out);
					count--;
					if (count == 0) break;
				}
			}
			else
			{
				outVector.erase(outVector.begin(), outVector.begin() + blockSize / 2 -1);
				outVector.erase(outVector.begin() + outVector.size() - blockSize / 2 -1, outVector.begin() + outVector.size());
				count = outVector.size();

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
			}
			break;
		}
	}
	return true;
}

void rootRaisedCosine(vector<t_real> &impulseResponse, int impulseResponseLength, double rollOffFactor, double samplingPeriod, double symbolPeriod, bool passiveFilterMode) {
	
	double gain{ 0 };
	for (int i = 0; i < impulseResponseLength; i++) {
		t_real t = -impulseResponseLength / 2 * samplingPeriod + i * samplingPeriod;
		t_real alpha = PI * t / symbolPeriod;
		if (0 == t) {
			impulseResponse[i] = (1 / symbolPeriod)*(1 + rollOffFactor * ((4 / PI) - 1));
		}
		else if (abs(t) == (symbolPeriod / (4 * rollOffFactor))) {
			impulseResponse[i] = (rollOffFactor / (symbolPeriod*sqrt(2)))*(1 + (2 / PI))*sin(PI / (4 * rollOffFactor)) + (1 - (2 / PI)*cos(PI / (4 * rollOffFactor)));
		}
		else {
			impulseResponse[i] = sin((alpha)*(1 - rollOffFactor)) + ((4 * rollOffFactor*t / symbolPeriod)*cos((alpha)*(1 + rollOffFactor)));
			impulseResponse[i] = (1 / symbolPeriod)* impulseResponse[i] / (alpha * (1 - ((4 * rollOffFactor * t / symbolPeriod) * (4 * rollOffFactor * t / symbolPeriod))));
		}

		gain = gain + impulseResponse[i];
	}

	for (int i = 0; i < impulseResponseLength; i++)
	{
		impulseResponse[i] = impulseResponse[i] * symbolPeriod;
	}

	if (passiveFilterMode)
	{
		for (int i = 0; i < impulseResponseLength; i++)
		{
			impulseResponse[i] = impulseResponse[i] / gain;
		}
	}
};
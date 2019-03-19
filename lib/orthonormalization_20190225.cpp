# include "orthonormalization_20190225.h"

# include <algorithm>
# include <complex>
# include <iostream>
# include <fstream>
# include <random>
# include <numeric>

using namespace std;


void Orthonormalization::initialize(void) {

	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());


	blockSize = outputSignals[0]->getBufferLength();
}


bool Orthonormalization::runBlock(void) {

	t_complex in;
	t_complex out;

	int space = outputSignals[0]->space();


	delayLineI.resize(blockSize);
	delayLineQ.resize(blockSize);
	delayLineQog.resize(blockSize);

	int ready = inputSignals[0]->ready();
	space = outputSignals[0]->space();
	int process = min(ready, space);


	if (process == 0) return false;
	
	for (int i = 0; i < process; i++)
	{
		inputSignals[0]->bufferGet(&in);
		delayLineI[aux] = real(in);
		delayLineQ[aux] = imag(in);

		aux++;
		if (aux == blockSize)
		{
			count = aux;
			aux = 0;

			PI = 0;
			PX = 0;
			for (int j = 0; j < blockSize; j++) 
			{
				PI += delayLineI[j] * delayLineI[j] / blockSize;
				PX += delayLineI[j] * delayLineQ[j] / blockSize;
			}
	
			for (int j = 0; j < blockSize; j++) delayLineQog[j] = delayLineQ[j] - delayLineI[j] * PX / PI;
			
			PQ = 0;
			for (int j = 0; j < blockSize; j++) PQ += delayLineQog[j] * delayLineQog[j] / blockSize;

			int process = outputSignals[0]->space() - outputSignals[0]->getInPosition();
			outputSignals[0]->setOutPosition(0);

			for (int j = 0; j < process; j++)
			{
				double Io = delayLineI[0] / sqrt(PI);
				double Qo = delayLineQog[0] / sqrt(PQ);

				delayLineI.erase(delayLineI.begin(), delayLineI.begin() + 1);
				delayLineQog.erase(delayLineQog.begin(), delayLineQog.begin() + 1);

				out = complex<double>(Io, Qo);
				outputSignals[0]->bufferPut(out);
				count--;
				if (count == 0) break;
			}

		}
	}



	return true;
}

# include "timing_deskew_20190225.h"

# include <complex>
# include "..\algorithms\fft\fft_20180208.h"

using namespace std;


void TimingDeskew::initialize(void) {

	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());

	blockSize = outputSignals[0]->getBufferLength();

	delayLineI.resize(blockSize);
	delayLineQ.resize(blockSize);
	delayLineIPrevious.resize(blockSize);
	delayLineQPrevious.resize(blockSize);
	f.resize(2 * blockSize);
	filterI.resize(2 * blockSize);
	filterQ.resize(2 * blockSize);

	double samplingPeriod = inputSignals[0]->getSamplingPeriod();
	double samplingFrequency = 1 / samplingPeriod;

	for (int i = 0; i < blockSize; i++) 
	{
		delayLineIPrevious[i] = 0.0;
		delayLineQPrevious[i] = 0.0;
	
	}

	for (int i = 0; i < 2 * blockSize; i++) {
		f[i] = samplingFrequency * (-(double)blockSize + i) / (blockSize * 2.0);
		
		filterI[i] = polar(1.0, -2 * PI * f[i] * skew[0]);
		filterQ[i] = polar(1.0, -2 * PI * f[i] * skew[1]);
	}
}


bool TimingDeskew::runBlock(void) {

	t_complex in;
	t_complex out;

	int space = outputSignals[0]->space();

	if (count != 0)
	{
		int process = outputSignals[0]->space() - outputSignals[0]->getInPosition();
		outputSignals[0]->setOutPosition(0);
		for (int j = 0; j < process; j++)
		{
			out = complex<double>(real(realFiltered[0]), -real(imagFiltered[0]));

			realFiltered.erase(realFiltered.begin(), realFiltered.begin() + 1);
			imagFiltered.erase(imagFiltered.begin(), imagFiltered.begin() + 1);

			outputSignals[0]->bufferPut(out);
			count--;
			if (count == 0) break;
		}
		if (count != 0) return false;
	}

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

			vector<t_complex> realIn;
			realIn.reserve(delayLineI.size() + delayLineIPrevious.size());
			realIn.insert(realIn.end(), delayLineIPrevious.begin(), delayLineIPrevious.end());
			realIn.insert(realIn.end(), delayLineI.begin(), delayLineI.end());
			vector<t_complex> imagIn;
			imagIn.reserve(delayLineI.size() + delayLineIPrevious.size());
			imagIn.insert(imagIn.end(), delayLineQPrevious.begin(), delayLineQPrevious.end());
			imagIn.insert(imagIn.end(), delayLineQ.begin(), delayLineQ.end());

			fft(realIn, 1, 1);
			fft(imagIn, 1, 1);

			realFiltered.resize(realIn.size());
			imagFiltered.resize(imagIn.size());
			
			for (size_t j = 0; j < realIn.size(); j++)
			{
				realFiltered[j] = (realIn[j]) * conj(filterI[j]);
				imagFiltered[j] = (imagIn[j]) * conj(filterQ[j]);
			}
			
			fft(realFiltered, -1, 1);
			fft(imagFiltered, -1, 1);

			realFiltered.erase(realFiltered.begin(), realFiltered.begin() + blockSize);
			imagFiltered.erase(imagFiltered.begin(), imagFiltered.begin() + blockSize);

			if (space != 512)
			{
				int a = 23;
			}

			int process = outputSignals[0]->space() - outputSignals[0]->getInPosition();
			outputSignals[0]->setOutPosition(0);

			for (int j = 0; j < process; j++)
			{
				out = complex<double>(real(realFiltered[0]), -real(imagFiltered[0]));

				realFiltered.erase(realFiltered.begin(), realFiltered.begin() + 1);
				imagFiltered.erase(imagFiltered.begin(), imagFiltered.begin() + 1);

				outputSignals[0]->bufferPut(out);
				count--;
			}

			delayLineIPrevious = delayLineI;
			delayLineQPrevious = delayLineQ;

			break;
		}
	}



	return true;
}

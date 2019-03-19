# include "../include/quantum_framer_20180815.h"

void calculateHeader(t_integer bPosition, t_binary &basisValue, t_binary &bitValue);

void QuantumFramer::initialize(void) 
{

	for (auto k : outputSignals)
	{
		k->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
		k->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	}

};

bool QuantumFramer::runBlock(void) 
{

	bool blockActive{ false };

	auto ready = inputSignals[0]->ready();

	auto space = MAX_BUFFER_LENGTH;
	for (auto k : outputSignals)
	{
		space = (space < k->space()) ? space : k->space();
	}

	int nFrames = ready % (2*(frameLength - headerLength));   // one bit in the frame needs two random bits (one bit value and one basis value)
	auto process = min(nFrames*2*frameLength , space);

	auto m{ 0 };
	while( m < process-1 )
	{
		blockActive = true;

		t_binary basisValue;
		t_binary bitValue;


		if (bitPosition < headerLength)
		{
			calculateHeader(bitPosition, basisValue, bitValue);

			for (auto k : outputSignals)
			{
				k->bufferPut(basisValue);
				k->bufferPut(bitValue);
			}
		}
		else
		{
			inputSignals[0]->bufferGet(&basisValue);
			inputSignals[0]->bufferGet(&bitValue);

			for (auto k : outputSignals)
			{
				k->bufferPut(basisValue);
				k->bufferPut(bitValue);
			}
		}
		m = m + 2;
		bitPosition++;
		bitPosition = bitPosition % frameLength;
	}
	
	return blockActive;
}

void calculateHeader(t_integer bPosition, t_binary &basisValue, t_binary &bitValue )
{
	basisValue = 0;
	bitValue = 0;

}
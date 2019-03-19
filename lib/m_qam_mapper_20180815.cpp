# include "../include/m_qam_mapper_20180815.h"


void MQamMapper::initialize(void){

	for (auto k : outputSignals)
	{
		k->setSymbolPeriod(log2(m) * inputSignals[0]->getSymbolPeriod());
		k->setSamplingPeriod(log2(m) * inputSignals[0]->getSamplingPeriod());
	}

}

bool MQamMapper::runBlock(void) {

	auto ready = inputSignals[0]->ready();

	auto space = MAX_BUFFER_LENGTH;
	for (auto k : outputSignals)
	{
		space = (space < k->space()) ? space : k->space();
	}

	if (firstTime) space = space - 1;						// the first output sample is going to be zero, to make the system causal

	auto length = (ready <= (2 * space)) ? ready : space; 

	if (length <= 0) return false;

	int nBinaryValues = (int)log2(m);

	for (auto k{ 0 }; k!=length; ++k) {

		t_binary binaryValue;
		inputSignals[0]->bufferGet(&binaryValue);
		auxSignalNumber = auxSignalNumber + (int) pow(2, nBinaryValues - 1 - auxBinaryValue) * binaryValue;
		auxBinaryValue++;

		if (auxBinaryValue == nBinaryValues) {

			if (firstTime)
			{
				for (auto s : outputSignals)
				{
					s->bufferPut((t_real)0);
				}
				firstTime = false;
			};

			unsigned int v{ 0 };
			auto vMax{ iqAmplitudes[0].size()-1 };
			for (auto s : outputSignals)
			{
				s->bufferPut(iqAmplitudes[auxSignalNumber][v]);
				if (v < vMax) v++;
			}

			auxBinaryValue = 0;
			auxSignalNumber = 0;
		}
	}

	return true;
}

void MQamMapper::setIqAmplitudes(vector<vector<t_real>> iqAmplitudesValues){
	m = (int) iqAmplitudesValues.size();
	iqAmplitudes.resize(m);
	iqAmplitudes = iqAmplitudesValues; 
};

void MQamMapper::setM(int mValue){
	m = mValue;

	iqAmplitudes.resize(m);
	switch (m) {
	case 4:
		iqAmplitudes = { { 1.0, 1.0 }, { -1.0, 1.0 }, { -1.0, -1.0 }, { 1.0, -1.0 } };

	case 16:
		iqAmplitudes = { { -3.0, -3.0 },{ -3.0, -1.0 },{ -3.0, 3.0 },{ -3.0, 1.0 },{ -1.0, -3.0 },{ -1.0, -1.0 },{ -1.0, 3.0 },{ -1.0, 1.0 },{ 3.0, -3.0 },{ 3.0, -1.0 }, { 3.0, 3.0 },{ 3.0, 1.0 },{ 1.0, -3.0 },{ 1.0, -1.0 },{ 1.0, 3.0 },{ 1.0, 1.0 } };
	};
}; 

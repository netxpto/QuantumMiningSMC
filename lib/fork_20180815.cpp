#include "../include/fork_20180815.h"

template<typename T>
void action(t_integer process, Block* blk)
{
	for (auto k=0; k != process; k++)
	{
		T aux;
		blk->inputSignals[0]->bufferGet(&aux);
		for (auto m : blk->outputSignals) m->bufferPut(aux);
	}
	return;
}

void Fork::initialize(void)
{

	for (auto k : outputSignals)
	{
		k->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
		k->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
		k->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	}

}

bool Fork::runBlock(void)
{

	auto ready = inputSignals[0]->ready();

	auto space = MAX_BUFFER_LENGTH;
	for (auto k : outputSignals) space = min(space, k->space());
		
	auto process = min(ready, space);

	if (process <= 0) return false;

	signal_value_type type = inputSignals[0]->getValueType();

	switch (type) 
	{
		case signal_value_type::t_binary:
			action<t_binary>(process, this);
			break;
		case signal_value_type::t_real:
			action<t_real>(process, this);
			break;
		case signal_value_type::t_complex:
			action<t_complex>(process, this);
			break;
		default:
			cout << "Erro 0: fork_20180815 - invalid signal_value_type\n";
	}

	return true;
}


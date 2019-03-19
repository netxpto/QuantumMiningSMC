# include "../include/time_delay_20180815.h"


template<typename T>
void action0(t_integer &space, double& sampPeriod, TimeDelay* blk)
{
	for (auto k : blk->outputSignals)
	{
		k->bufferPut((T)0);
		blk->setTimeDelay(blk->getTimeDelay() - sampPeriod);
		space--;
	}
}

template<typename T>
void action1(t_unsigned process, TimeDelay* blk)
{
	for (auto k{ 0 }; k != process; ++k) {
		T aux;
		blk->inputSignals[0]->bufferGet(&aux);
		for (auto m : blk->outputSignals)
		{
			m->bufferPut(aux);
		}
	}
}

void TimeDelay::initialize(void) {

	for (auto k : outputSignals)
	{
		k->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
		k->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	}

};

bool TimeDelay::runBlock(void) {


	bool activeBlock{ false };

	auto space = MAX_BUFFER_LENGTH;
	for (auto k : outputSignals)
	{
		space = (space < k->space()) ? space : k->space();
	}

	while ((space!=0) && (timeDelay > 0))
	{
		activeBlock = true;

		auto sampPeriod = inputSignals[0]->getSamplingPeriod();

		switch (inputSignals[0]->getValueType()) {
			case signal_value_type::t_binary:
				action0<t_binary>(space, sampPeriod, this);
				break;
			case signal_value_type::t_real:
				action0<t_real>(space, sampPeriod, this);
				break;
			case signal_value_type::t_complex:
				action0<t_complex>(space, sampPeriod, this);
				break;
			default:
				cout << "Erro 0: time_delay_20180815 - invalid signal_value_type\n";
		}
	}
	
	auto ready = inputSignals[0]->ready();

	auto process = (ready <= space) ? ready : space;

	if (process <= 0) return activeBlock;

	switch (inputSignals[0]->getValueType())
	{
		case signal_value_type::t_binary:
			action1<t_binary>(process, this);
			break;
		case signal_value_type::t_real:
			action1<t_real>(process, this);
			break;
		case signal_value_type::t_complex:
			action1<t_complex>(process, this);
			break;
		default:
			cout << "Erro 1: time_delay_20180815 - invalid signal_value_type\n";
	}
	
	return true;
}


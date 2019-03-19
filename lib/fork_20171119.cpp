#include "netxpto.h"
#include "fork_20171119.h"

using namespace std;

void Fork::initialize(void)
{
	outputSignals[0]->setSymbolPeriod ( inputSignals[0]->symbolPeriod);
	outputSignals[0]->setSamplingPeriod ( inputSignals[0]->samplingPeriod);
	outputSignals[0]->setSamplesPerSymbol ( inputSignals[0]->samplesPerSymbol);
	outputSignals[0]->setFirstValueToBeSaved ( inputSignals[0]->getFirstValueToBeSaved());

	outputSignals[1]->setSymbolPeriod(inputSignals[0]->symbolPeriod);
	outputSignals[1]->setSamplingPeriod(inputSignals[0]->samplingPeriod);
	outputSignals[1]->setSamplesPerSymbol(inputSignals[0]->samplesPerSymbol);
	outputSignals[1]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	
}



bool Fork::runBlock(void)
{

	int ready = inputSignals[0]->ready();
	int space1 = outputSignals[0]->space();
	int space2 = outputSignals[1]->space();

	int space = min(space1, space2);
	int process = min(ready, space);

	if (process <= 0) return false;

	
	signal_value_type type = inputSignals[0]->getValueType();

	switch(type) {
		case BinaryValue:
		{
			for (int i = 0; i < process; i++)
			{
				t_binary inSignal;
				inputSignals[0]->bufferGet(&inSignal);
				outputSignals[0]->bufferPut((t_binary)(inSignal));
				outputSignals[1]->bufferPut((t_binary)(inSignal));
			}
			break;
		}

		case IntegerValue:
		{
			for (int i = 0; i < process; i++)
			{
				t_integer inSignal;
				inputSignals[0]->bufferGet(&inSignal);
				outputSignals[0]->bufferPut((t_integer)(inSignal));
				outputSignals[1]->bufferPut((t_integer)(inSignal));
			}
			break;
		}

		case RealValue:
		{
			for (int i = 0; i < process; i++)
			{
				t_real inSignal;
				inputSignals[0]->bufferGet(&inSignal);
				outputSignals[0]->bufferPut((t_real)(inSignal));
				outputSignals[1]->bufferPut((t_real)(inSignal));
			}
			break;
		}

		case ComplexValue:
		{
			for (int i = 0; i < process; i++)
			{
				t_complex inSignal;
				inputSignals[0]->bufferGet(&inSignal);
				outputSignals[0]->bufferPut((t_complex)(inSignal));
				outputSignals[1]->bufferPut((t_complex)(inSignal));
			}
			break;
		}

		case ComplexValueXY:
		{
			for (int i = 0; i < process; i++)
			{
				t_complex_xy inSignal;
				inputSignals[0]->bufferGet(&inSignal);
				outputSignals[0]->bufferPut((t_complex_xy)(inSignal));
				outputSignals[1]->bufferPut((t_complex_xy)(inSignal));
			}
			break;
		}

		case PhotonValue:
		{
			for (int i = 0; i < process; i++)
			{
				t_photon inSignal;
				inputSignals[0]->bufferGet(&inSignal);
				outputSignals[0]->bufferPut((t_photon)(inSignal));
				outputSignals[1]->bufferPut((t_photon)(inSignal));
			}
			break;
		}

		case PhotonValueMP:
		{
			for (int i = 0; i < process; i++)
			{
				t_photon_mp inSignal;
				inputSignals[0]->bufferGet(&inSignal);
				outputSignals[0]->bufferPut((t_photon_mp)(inSignal));
				outputSignals[1]->bufferPut((t_photon_mp)(inSignal));
			}
			break;
		}

		case Message:
		{
			for (int i = 0; i < process; i++)
			{
				t_message inSignal;
				inputSignals[0]->bufferGet(&inSignal);
				outputSignals[0]->bufferPut((t_message)(inSignal));
				outputSignals[1]->bufferPut((t_message)(inSignal));
			}
			break;
		}
		default: std::cout << "Fork: Invalid Input Signal\n";
	}
	return true;
}


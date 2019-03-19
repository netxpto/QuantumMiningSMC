# include <algorithm>	// std::min

# include "netxpto.h"
# include "iq_modulator_20180828.h"
# include "fft_20180208.h"


using namespace std;

/* 2016-07-05
IqModulator::IqModulator(vector <Signal *> &InputSig, vector <Signal *> &OutputSig) {

numberOfInputSignals = InputSig.size();
numberOfOutputSignals = OutputSig.size();

inputSignals = InputSig;
outputSignals = OutputSig;


outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

outputSignals[0]->centralWavelength = outputOpticalWavelength;
outputSignals[0]->centralFrequency = outputOpticalFrequency;

}*/

void IqModulator::initialize(void) {

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}

bool IqModulator::runBlock(void)
{
	int ready1 = inputSignals[0]->ready();
	int ready2 = inputSignals[1]->ready();
	int ready = min(ready1, ready2);

	int space = outputSignals[0]->space();
	int process = min(ready, space);
	if (process == 0) return false;

	modulator type = modulatorType;

	switch (type)
	{
	case ideal:
		for (int i = 0; i < process; i++)
		{
			t_real it;
			t_real qt;
			t_complex outComplexSignalIdeal;

			inputSignals[0]->bufferGet(&it);
			inputSignals[1]->bufferGet(&qt);

			outComplexSignalIdeal = reIm2Complex(it,qt); // Complex Signal (Ideal IQ modulator)
			outputSignals[0]->bufferPut(t_complex(outComplexSignalIdeal));
		}
		break;
	case mzm:
		for (int i = 0; i < process; i++)
		{
			t_complex complexSignalMzm;
			t_complex outComplexSignalMzm;

			t_real i_t;
			t_real q_t;
			t_real phi_it;
			t_real phi_qt;
			double bias = vpi / 2;

			inputSignals[0]->bufferGet(&complexSignalMzm);


			i_t = real(complexSignalMzm); // Real Part of complex input signal
			q_t = imag(complexSignalMzm); // Imaginary part of the input signal

			phi_it = (i_t + bias)*PI / vpi; 
			phi_qt = (q_t + bias)*PI / vpi;

			outComplexSignalMzm = { (cos(phi_it / 2)) , (cos(phi_qt / 2)) }; // MZM field transfer function 

			outputSignals[0]->bufferPut(t_complex(outComplexSignalMzm));
		}
		break;
	default:
		break;
	}



	return true;
}
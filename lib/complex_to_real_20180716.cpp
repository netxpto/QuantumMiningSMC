# include "netxpto_20180418.h"
# include "complex_to_real_20180716.h"

using namespace std;



void ComplexToReal::initialize(void) {

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->
	                  getFirstValueToBeSaved());
	outputSignals[1]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[1]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[1]->setFirstValueToBeSaved(inputSignals[0]->
		getFirstValueToBeSaved());
}



bool ComplexToReal::runBlock(void) {
		
	int ready = inputSignals[0]->ready();
	
	//int space0 = outputSignals[0]->space();
	int space = outputSignals[0]->space();

	//int space = min(space0, space1);

	int process = min(ready, space);
	
	if (process == 0) return false;
	
	// Signal type verification
	signal_value_type sType     = inputSignals[0]->getValueType();
	signal_value_type sTypeOut0 = outputSignals[0]->getValueType();
	signal_value_type sTypeOut1 = outputSignals[1]->getValueType();


	// Only complex output signals are acceptable.
	if (sType != ComplexValue){
		cout << "ERROR: complex_to_real.cpp (input signal "
		"signal_value_type not ComplexValue)\n";
		return false;
	}
	// Only real input signals are acceptable.
	if (sTypeOut0 != RealValue){
		cout << "ERROR: real_to_complex.cpp (input signal0 "
			"signal_value_type not RealValue)\n";
		return false;
	}
	// Only real input signals are acceptable. 
	if (sTypeOut1 != RealValue) {
		cout << "ERROR: real_to_complex.cpp (input signal1 "
			"signal_value_type not RealValue)\n";
		return false;
	}
	
	
	// Buffer processing
	t_real re, im;
	t_complex valueX(0.0, 0.0);
	for (int i = 0; i < process; i++) {
		
		inputSignals[0]->bufferGet(&valueX);
		
		re = real(valueX);
		im = imag(valueX);

		outputSignals[0]->bufferPut(re);
		outputSignals[1]->bufferPut(im);
	}

	return true;
}

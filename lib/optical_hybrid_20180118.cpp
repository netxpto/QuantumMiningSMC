#include <algorithm>
#include <complex>
#include <iostream>
#include <fstream>

#include "netxpto_20180418.h"
#include "optical_hybrid_20180118.h"


void OpticalHybrid::initialize(void){

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	outputSignals[0]->setCentralWavelength(inputSignals[0]->getCentralWavelength());
	outputSignals[0]->setCentralFrequency(inputSignals[0]->getCentralFrequency());

	outputSignals[1]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[1]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[1]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	outputSignals[1]->setCentralWavelength(inputSignals[1]->getCentralWavelength());
	outputSignals[1]->setCentralFrequency(inputSignals[1]->getCentralFrequency());

	outputSignals[2]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[2]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[2]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	outputSignals[2]->setCentralWavelength(inputSignals[0]->getCentralWavelength());
	outputSignals[2]->setCentralFrequency(inputSignals[0]->getCentralFrequency());

	outputSignals[3]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[3]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[3]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	outputSignals[3]->setCentralWavelength(inputSignals[1]->getCentralWavelength());
	outputSignals[3]->setCentralFrequency(inputSignals[1]->getCentralFrequency());
}


bool OpticalHybrid::runBlock(void){

	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();
	int ready = min(ready0, ready1);

	int space0 = outputSignals[0]->space();
	int space1 = outputSignals[1]->space();
	int space2 = outputSignals[2]->space();
	int space3 = outputSignals[3]->space();
	int spacea = min(space0, space1);
	int spaceb = min(space2, space3);
	int space = min(spacea, spaceb);

	int process = min(ready, space);

	if (process == 0) return false;

	complex<double> imaginary(0, 1);
		 
	complex<double> div(0.5, 0);

	for (int i = 0; i < process; i++) {

		t_complex ina;
		t_complex inb;
		inputSignals[0]->bufferGet(&ina);
		inputSignals[1]->bufferGet(&inb);

		t_complex outa = div * ina + div * inb;
		t_complex outb = div * ina - div * inb;
		t_complex outc = div * ina + imaginary*div * inb;
		t_complex outd = div * ina - imaginary*div * inb;
		outputSignals[0]->bufferPut((t_complex) outa);
		outputSignals[1]->bufferPut((t_complex) outb);
		outputSignals[2]->bufferPut((t_complex) outc);
		outputSignals[3]->bufferPut((t_complex) outd);

	}
	return true;
}

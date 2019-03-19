# include "probability_estimator_20180124.h"

ProbabilityEstimator::~ProbabilityEstimator() {

	double probabilityOf0 = (double)(numberOf0 / numberOfReceivedBits);
	double probabilityOf1 = (double)(numberOf1 / numberOfReceivedBits);
	
	double errorOf0 = (double)(zScore * sqrt((probabilityOf0*(1 - probabilityOf0)) / numberOfReceivedBits));
	double errorOf1 = (double)(zScore * sqrt((probabilityOf1*(1 - probabilityOf1)) / numberOfReceivedBits));
	

	/*Outputing a .txt report*/
	ofstream myfile;
	myfile.open(fileName);
	myfile << "Number of received Bits: " << numberOfReceivedBits << "\n";
	myfile << "Error Margin of Prob0: " << (100*errorOf0) << " % \n";
	myfile << "Error Margin of Prob1: " << (100*errorOf1) << " % \n";
	myfile << "Probability Of 0: " << (100*probabilityOf0) << " % \n";
	myfile << "Probability Of 1: " << (100*probabilityOf1) << " % \n";

	myfile.close();

	return;

}

void ProbabilityEstimator::initialize() {

	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}


bool ProbabilityEstimator::runBlock(void) {

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process <= 0) return false;

	for (auto i = 0; i < process; i++) {
		
		t_binary inValue;
		inputSignals[0]->bufferGet(&inValue);

		if (inValue == 0) {
			numberOf0++;
			numberOfReceivedBits++;
		}
		if (inValue == 1) {
			numberOf1++;
			numberOfReceivedBits++;
		}

		outputSignals[0]->bufferPut((t_binary)inValue);

	}

	return true;
}
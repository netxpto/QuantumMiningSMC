# include "../include/quantum_bit_error_rate_20181203.h"

void QuantumBitErrorRate::initialize(void) {
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}

bool QuantumBitErrorRate::runBlock(void) {
	/* Computing z */ // This code converges in below 10 steps, exactness chosen in order to achieve this rapid convergence
	if (firstPass)
	{
		firstPass = 0;
		double x1 = -2;
		double x2 = 2;
		double x3 = x2 - (erf(x2 / sqrt(2)) + 1 - alpha)*(x2 - x1) / (erf(x2 / sqrt(2)) - erf(x1 / sqrt(2)));
		double exacteness = 1e-15;

		while (abs(erf(x3 / sqrt(2)) + 1 - alpha) > exacteness)
		{
			x3 = x2 - (erf(x2 / sqrt(2)) + 1 - alpha)*(x2 - x1) / (erf(x2 / sqrt(2)) - erf(x1 / sqrt(2)));
			x1 = x2;
			x2 = x3;
		}

		z = -x3;

	}

	int ready{ MAX_BUFFER_LENGTH };
	for (auto k : inputSignals) {
		ready = min(k->ready(), ready);
	}
	int space = outputSignals[0]->space();
	int process = min(ready, space);

	std::vector<char> values(dataSequence.begin(), dataSequence.end());
	int valuesSize = (int)values.size();

	if (process == 0)
	{
		/* Calculating bounds */
		double UpperBound = QBER + 1 / sqrt(receivedBits) * z  * sqrt(QBER*(1 - QBER)) + 1 / (3 * receivedBits)*(2 * z * z * (1 / 2 - QBER) + (2 - QBER));
		double LowerBound = QBER - 1 / sqrt(receivedBits) * z  * sqrt(QBER*(1 - QBER)) + 1 / (3 * receivedBits)*(2 * z * z * (1 / 2 - QBER) - (1 + QBER));

		/* Outputting a .txt report*/
		ofstream myfile;
		myfile.open(outputSignals[0]->getFolderName() + "/QBER.txt");
		myfile << "QBER = " << QBER*100 << " %\n";
		myfile << "Upper and lower confidence bounds for " << (1 - alpha) * 100 << "% confidence level \n";
		myfile << "Upper Bound = " << UpperBound * 100 << " %\n";
		myfile << "Lower Bound = " << LowerBound * 100 << " %\n";
		myfile << "Number of errors = " << receivedBits - coincidences << "\n";
		myfile << "Number of received bits = " << receivedBits << "\n";
		myfile.close();
		return false;
	}

	for (long int i = 0; i < process; i++) {

		t_real signalValue;
		inputSignals[0]->bufferGet(&signalValue);

		t_real reset{ 0.0 };
		if (inputSignals.size() > 1) 
			inputSignals[1]->bufferGet(&reset);
		
		t_real valueToCompare = (t_real)(values[posDataSeq]-'0');
		
		posDataSeq++;
		posDataSeq = posDataSeq % valuesSize;

		receivedBits++;
		if (signalValue == valueToCompare)
			coincidences++;

		if (window < 0) {
			QBER = (receivedBits - coincidences) / receivedBits;

			if (reset == 1.0) {

				n++;

				ostringstream oss;
				oss << outputSignals[0]->getFolderName() + "/midreport" << n << ".txt";
				string filename = oss.str();

				/* Calculating bounds */

				double UpperBound = QBER + 1 / sqrt(receivedBits) * z  * sqrt(QBER*(1 - QBER)) + 1 / (3 * receivedBits)*(2 * z * z * (1 / 2 - QBER) + (2 - QBER));
				double LowerBound = QBER - 1 / sqrt(receivedBits) * z  * sqrt(QBER*(1 - QBER)) + 1 / (3 * receivedBits)*(2 * z * z * (1 / 2 - QBER) - (1 + QBER));

				/* Outputting a .txt report*/
				ofstream myfile;
				myfile.open(filename);
				myfile << "QBER = " << QBER * 100 << " %\n";
				myfile << "Upper and lower confidence bounds for " << (1 - alpha) * 100 << "% confidence level \n";
				myfile << "Upper Bound = " << UpperBound * 100 << " %\n";
				myfile << "Lower Bound = " << LowerBound * 100 << " %\n";
				myfile << "Number of errors = " << receivedBits - coincidences << "\n";
				myfile << "Number of received bits = " << receivedBits << "\n";
				myfile.close();

				receivedBits = 0;
				coincidences = 0;
				QBER = 0.0;
			}
			outputSignals[0]->bufferPut(QBER);
		}
		else
			cout << "ERROR QBER: this mode is not implemented";

		}
		


	return true;
}
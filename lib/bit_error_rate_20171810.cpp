#include <algorithm>
#include <complex>

#include "netxpto_20180418.h"
#include "bit_error_rate_20171810.h"

void BitErrorRate::initialize(void){
	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}


bool BitErrorRate::runBlock(void){

	/* Computing z */ // This code converges in below 10 steps, exactness chosen in order to achieve this rapid convergence
	if (firstPass)
	{
		firstPass = 0;
		double x1 = -2;
		double x2 = 2;
		double x3 = x2 - (erf(x2 / sqrt(2)) + 1 - alpha)*(x2 - x1) / (erf(x2 / sqrt(2)) - erf(x1 / sqrt(2)));
		double exacteness = 1e-15;

		while (abs(erf(x3 / sqrt(2)) + 1 - alpha)>exacteness)
		{
			x3 = x2 - (erf(x2 / sqrt(2)) + 1 - alpha)*(x2 - x1) / (erf(x2 / sqrt(2)) - erf(x1 / sqrt(2)));
			x1 = x2;
			x2 = x3;
		}

		z = -x3;

	}

	int ready1 = inputSignals[0]->ready();
	int ready2 = inputSignals[1]->ready();
	int ready = min(ready1, ready2);
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	
	/* Outputting final report */

	if (process == 0)
	{

		/* Calculating BER and bounds */

		double BER = (receivedBits - coincidences) / receivedBits;
        
        if (BER==0) {
           // cout << "ERRO: bit_error_rate.cpp (null BER, not enough samples)" << "\n";
        }

		double UpperBound = BER + 1 / sqrt(receivedBits) * z  * sqrt(BER*(1 - BER)) + 1 / (3 * receivedBits)*(2 * z * z * (1 / 2 - BER) + (2 - BER));
		double LowerBound = BER - 1 / sqrt(receivedBits) * z  * sqrt(BER*(1 - BER)) + 1 / (3 * receivedBits)*(2 * z * z * (1 / 2 - BER) - (1 + BER));

        if (LowerBound<lowestMinorant) {
            LowerBound=lowestMinorant;
        }
        
		/* Outputting a .txt report*/
		ofstream myfile;
		myfile.open(outputSignals[0]->getFolderName() + "/BER.txt");
		myfile << "BER= " << BER << "\n";
		myfile << "Upper and lower confidence bounds for " << (1 - alpha) * 100 << "% confidence level \n";
		myfile << "Upper Bound= " << UpperBound << "\n";
		myfile << "Lower Bound= " << LowerBound << "\n";
		myfile << "Number of received bits =" << receivedBits << "\n";
		myfile.close();
		return false;
	}



	for (long int i = 0; i < process; i++) {

		t_binary signalValue;
		inputSignals[0]->bufferGet(&signalValue);
		t_binary SignalValue;
		inputSignals[1]->bufferGet(&SignalValue);

		/* Outputting mid reports */
		if (m > 0)
		{
			if ((remainder(receivedBits, m) == 0) & (receivedBits > 0))
			{
				n++;

				ostringstream oss;
				oss << "midreport" << n << ".txt";
				string filename = oss.str();

				/* Calculating BER and bounds */

				double BER;
				BER = (receivedBits - coincidences) / receivedBits;

				double UpperBound = BER + 1 / sqrt(receivedBits) * z  * sqrt(BER*(1 - BER)) + 1 / (3 * receivedBits)*(2 * z * z * (1 / 2 - BER) + (2 - BER));
				double LowerBound = BER - 1 / sqrt(receivedBits) * z  * sqrt(BER*(1 - BER)) + 1 / (3 * receivedBits)*(2 * z * z * (1 / 2 - BER) - (1 + BER));

				/* Outputting a .txt report*/
				ofstream myfile;
				myfile.open(filename);
				myfile << "BER= " << BER << "\n";
				myfile << "Upper and lower confidence bounds for" << 1 - alpha << "confidence level \n";
				myfile << "Upper Bound= " << UpperBound << "\n";
				myfile << "Lower Bound= " << LowerBound << "\n";
				myfile << "Number of received bits =" << receivedBits << "\n";
				myfile.close();
			}
		}

		receivedBits++;
			if (signalValue == SignalValue)
			{
				coincidences++;
				outputSignals[0]->bufferPut((t_binary)1);
			}
			else
			{
				cout << "received bits: " << receivedBits << "\n";
				outputSignals[0]->bufferPut((t_binary)0);
			}
	}
	return true;
}



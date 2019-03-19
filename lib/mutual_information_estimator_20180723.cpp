#include "mutual_information_estimator_20180723.h"

void MutualInformationEstimator::initialize(void) {
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
};

bool MutualInformationEstimator::runBlock(void) {

	/* Computing z */ // This code converges in below 10 steps, exactness chosen in order to achieve this rapid convergence
	if (firstPass)
	{
		firstPass = false;
		double x1 = -2;
		double x2 = 2;
		double x3 = x2 - (erf(x2 / sqrt(2)) + 1 - alpha_bounds)*(x2 - x1) / (erf(x2 / sqrt(2)) - erf(x1 / sqrt(2)));
		double exacteness = 1e-15;

		while (abs(erf(x3 / sqrt(2)) + 1 - alpha_bounds)>exacteness)
		{
			x3 = x2 - (erf(x2 / sqrt(2)) + 1 - alpha_bounds)*(x2 - x1) / (erf(x2 / sqrt(2)) - erf(x1 / sqrt(2)));
			x1 = x2;
			x2 = x3;
		}

		z = -x3;

	}

	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();
	int ready = min(ready0, ready1);

	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process <= 0) {
		double pError = p / receivedBits;
		double condEntropyYX = pError * log2(1 / pError) + (1-pError)*log2(1/(1-pError));

		double pAlpha = alpha / receivedBits;
		double probValue = (1 - pError)*pAlpha + pError * (1 - pAlpha);
		double entropyY = probValue * log2(1 / probValue) + (1-probValue)*log2(1/(1-probValue));

		double mI = entropyY - condEntropyYX;
		
		double UpperBoundMI = mI + 1 / sqrt(receivedBits) * z  * sqrt(mI*(1 - mI)) + 1 / (3 * receivedBits)*(2 * z * z * (1 / 2 - mI) + (2 - mI));
		double LowerBoundMI = mI - 1 / sqrt(receivedBits) * z  * sqrt(mI*(1 - mI)) + 1 / (3 * receivedBits)*(2 * z * z * (1 / 2 - mI) - (1 + mI));

		double UpperBoundCE = condEntropyYX + 1 / sqrt(receivedBits) * z  * sqrt(condEntropyYX*(1 - condEntropyYX)) + 1 / (3 * receivedBits)*(2 * z * z * (1 / 2 - condEntropyYX) + (2 - condEntropyYX));
		double LowerBoundCE = condEntropyYX - 1 / sqrt(receivedBits) * z  * sqrt(condEntropyYX*(1 - condEntropyYX)) + 1 / (3 * receivedBits)*(2 * z * z * (1 / 2 - condEntropyYX) - (1 + condEntropyYX));

		double UpperBoundE = entropyY + 1 / sqrt(receivedBits) * z  * sqrt(entropyY*(1 - entropyY)) + 1 / (3 * receivedBits)*(2 * z * z * (1 / 2 - entropyY) + (2 - entropyY));
		double LowerBoundE = entropyY - 1 / sqrt(receivedBits) * z  * sqrt(entropyY*(1 - entropyY)) + 1 / (3 * receivedBits)*(2 * z * z * (1 / 2 - entropyY) - (1 + entropyY));

		/* Outputting a .txt report*/
		ofstream myfile;
		myfile.open(outputSignals[0]->getFolderName() + "/MI.txt");
		myfile << "Mutual Information = " << mI << "\n";
		myfile << "UpperBound = " << UpperBoundMI << " \n";
		myfile << "LowerBound = " << LowerBoundMI << " \n";
		myfile << "------------------------------------- \n\n";
		myfile << "H(Y|X) = " << condEntropyYX << " \n";
		myfile << "UpperBound = " << UpperBoundCE << " \n";
		myfile << "LowerBound = " << LowerBoundCE << " \n";
		myfile << "p = " << pError << " \n";
		myfile << "------------------------------------- \n\n";
		myfile << "H(Y) = " << entropyY << " \n";
		myfile << "UpperBound = " << UpperBoundE << " \n";
		myfile << "LowerBound = " << LowerBoundE << " \n";
		myfile << "p'.alpha + p.alpha' = " << probValue << " \n\n";

		myfile << "Number of received bits = " << receivedBits << "\n";
		myfile.close();

		return false;
	}

	else {
		for (auto i = 0; i < process; i++) {
			t_binary bitX;
			inputSignals[0]->bufferGet(&bitX);
			t_binary bitY;
			inputSignals[1]->bufferGet(&bitY);
			t_binary bitOut{ 0 };

			receivedBits++;

			if (m > 0) {
				if ((remainder(receivedBits, m) == 0))
				{
					n++;

					ostringstream oss;
					oss << "\midreport" << n << ".txt";
					string filename = oss.str();

					/* Calculating MI */

					double pError = double(p / receivedBits);
					double condEntropyYX = pError * log2(1 / pError);

					double pAlpha = alpha / receivedBits;
					double probValue = (1 - pError)*pAlpha + pError * (1 - pAlpha);
					double entropyY = probValue * log2(1 / probValue);

					double mI = entropyY - condEntropyYX;

					/* Outputting a .txt report*/
					ofstream myfile;
					myfile.open(outputSignals[0]->getFolderName() + filename);
					myfile << "Mutual Information = " << mI << "\n";
					myfile << "Channel Error Probability = " << pError * 100 << " % \n";
					myfile << "Probability of X=0 (alpha) = " << pAlpha * 100 << " % \n";
					myfile << "Number of received bits = " << receivedBits << "\n";

					myfile.close();
				}
			}
		
			/*alpha :x=0*/
			if (bitX == 0) 
				alpha++;
			/*p : error*/
			if (bitX != bitY) {
				p++;
				bitOut = 1;
			}
			else
				bitOut = 0;

			outputSignals[0]->bufferPut(bitOut);
					
			
		}
	}

	return true;
};
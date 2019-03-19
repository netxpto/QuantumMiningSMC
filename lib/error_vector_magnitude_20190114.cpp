#include "../include/error_vector_magnitude_20190114.h"

void ErrorVectorMagnitude::initialize(void) {
	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}


bool ErrorVectorMagnitude::runBlock(void) {

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	/* Outputting final report */

	if (process == 0)
	{
		if (firstPass)
		{
			int symbolFrequency = round(1/inputSignals[0]->getSymbolPeriod());
			string a = outputSignals[0]->getFileName();
			string b = a.substr(0, a.length() - 4);
			string c = to_string(symbolFrequency);

			// firstPass = false;
			/* Outputting a .txt report*/
			ofstream myfile;
			myfile.open(outputSignals[0]->getFolderName() + "/EVM" + b + "_" + c + ".txt");
			/*  myfile << "EVM = " << EVM * 100 << "\n";
				myfile << "baud rate = " << 1.0 / (inputSignals[0]->getSymbolPeriod()) << "\n";
				myfile << receivedBits << " bits received" << "\n"; */
			myfile << EVM * 100 << "\n";
			myfile.close();
		}
		return false;
	}

	/*      Outputting mid reports
		if (m > 0)
		{
			if ((remainder(receivedBits_mid, m) == 0) & (receivedBits_mid > 0))
			{
				n++;

				ostringstream oss;
				oss << "midreportEVM" << n << ".txt";
				string filename = oss.str();

				/* Outputting a .txt report
				ofstream myfile;
				myfile.open(filename);
				myfile << "EVM = " << EVM << "\n";
				myfile.close();

				if (midRepType == Reset) {
					receivedBits_mid = 0;
					coincidences_mid = 0;
				}
			}
		} */

	t_complex signalValue;
	t_complex errorVector;

	for (long int i = 0; i < process; i++) {

		inputSignals[0]->bufferGet(&signalValue);


		receivedSymbols++;
		receivedBits_mid++;
		if (real(signalValue) > 0 && imag(signalValue) > 0)
		{
			errorVector = signalValue - referenceAmplitude * complex<double>(1.0, 1.0);
			if (receivedSymbols<=numberOfSymbols) EVM = ((receivedSymbols - 1)*EVM + abs(errorVector) / referenceAmplitude) / receivedSymbols;
		}
		else if (real(signalValue) > 0 && imag(signalValue) < 0)
		{
			errorVector = signalValue - referenceAmplitude * complex<double>(1.0, -1.0);
			if (receivedSymbols <= numberOfSymbols) EVM = ((receivedSymbols - 1)*EVM + abs(errorVector) / referenceAmplitude) / receivedSymbols;
		}
		else if (real(signalValue) < 0 && imag(signalValue) < 0)
		{
			errorVector = signalValue - referenceAmplitude * complex<double>(-1.0, -1.0);
			if (receivedSymbols <= numberOfSymbols) EVM = ((receivedSymbols - 1)*EVM + abs(errorVector) / referenceAmplitude) / receivedSymbols;
		}
		else if (real(signalValue) < 0 && imag(signalValue) > 0)
		{
			errorVector = signalValue - referenceAmplitude * complex<double>(-1.0, 1.0);
			if (receivedSymbols <= numberOfSymbols) EVM = ((receivedSymbols - 1)*EVM + abs(errorVector) / referenceAmplitude) / receivedSymbols;
		}
		outputSignals[0]->bufferPut(signalValue);
	}
	return true;
}



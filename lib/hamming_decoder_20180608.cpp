#include <algorithm>
#include <iostream>
#include <Windows.h>

#include "netxpto_20180418.h"
#include "hamming_decoder_20180608.h"

using namespace std;

void HammingDeCoder::initialize(void) {
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
}

bool HammingDeCoder::runBlock(void) {
	bool alive{ false };

	/* Update the parity bits according to the defined nBits and kBits */
	setParityBits();
	if (parityBits == 0) {
		OutputDebugString("ERROR: (Hamming DeCoder) Invalid conmbination of (n, k) Hamming Code. \n");

		return alive;
	}

	/* Determine n and k parameters according to selected parity for Hamming Code */
	int hamming_n = nBits;
	int hamming_k = kBits;

	/* Avaiable bits on input buffer */
	int ready = inputSignals[0]->ready();

	/* Avaiable space on output buffer */
	int space = outputSignals[0]->space();

	/* Cycle to process data */
	while ((ready >= hamming_k) && (space >= hamming_n)) {
		/* Vectors for input/output data */
		std::vector<int> inputData(hamming_n);
		std::vector<int> outputData(hamming_k);
		std::vector<int> errorData(hamming_n - hamming_k);

		// Get the first k bits
		for (int k = 0; k < hamming_n; k++) {
			int in;
			inputSignals[0]->bufferGet(&in);

			inputData[k] = in;
		}

		// Multiply Vector by Matrix (in order to detect bit errors)
		int sumError = 0;
		for (int k = 0; k < hamming_n - hamming_k; k++) {
			for (int m = 0; m < hamming_n; m++) {
				errorData[k] += (H[m][k] * inputData[m]);

				if (errorData[k] > 1) {
					errorData[k] = 0;
				}
			}

			sumError += errorData[k];
		}

		/* Check if an error was detected */
		if (sumError != 0) {
			/* Detect were the error was produced */
			int errorLocation = -1;

			/* Loacte error */
			for (int m = getParityBits(); m < hamming_n; m++) {
				int errorValidation = 0;
				for (int k = 0; k < hamming_n - hamming_k; k++) {
					if (H[m][k] == errorData[k]) {
						errorValidation += 1;
					}
				}

				/* Save the location were the error was detected */
				if (errorValidation == getParityBits()) {
					errorLocation = m;
					break;
				}
			}

			/* Correct the previously detected error */
			if (errorLocation != -1) {
				inputData[errorLocation] += 1;
				if (inputData[errorLocation] > 1) {
					inputData[errorLocation] = 0;
				}
			} /* If the error was detected in the parity bits, do nothing */
		}

		/* Filter data for output */
		for (int k = 0; k < hamming_k; k++) {
			outputData[k] = inputData[hamming_n - hamming_k + k];
		}

		// Store data in output
		for (int k = 0; k < hamming_k; k++) {
			outputSignals[0]->bufferPut(outputData[k]);
		}

		ready = inputSignals[0]->ready();
		space = outputSignals[0]->space();
	}

	return alive;
}

void HammingDeCoder::setParityBits(void) {
	/* Save the selected parity bits for the Hamming Code */
	parityBits = getNBits() - getKBits();

	for (int k = 0; k < (int)size(parity_array); k++) {
		if (parity_array[k] == parityBits) {
			if ((k_array[k] == kBits) && (n_array[k] == nBits)) {
				/* Calculate the G matrix according to the selected parity bits */
				setHMatrix(parityBits);
				return;
			}
		}
	}

	parityBits = 0;
}

int HammingDeCoder::getNBits() {
	return nBits;
}

void HammingDeCoder::setNBits(int s_n) {
	/* Save the selected Code Word length for the Hamming Code */
	nBits = s_n;
}

int HammingDeCoder::getKBits() {
	return kBits;
}

void HammingDeCoder::setKBits(int s_n) {
	/* Save the selected Data length for the Hamming Code */
	kBits = s_n;
}

int HammingDeCoder::getParityBits() {
	return parityBits;
}

void HammingDeCoder::setHMatrix(int s_n) {
	/* Determine n and k parameters according to selected parity for Hamming Code */
	const int hamming_n = n_array[getParityBits() - 2];

	/* Auxiliary G Matrix to perform calculations */
	vector<vector<int>> H_aux(hamming_n, vector<int>(getParityBits()));

	/* Parity Part of G matrix */
	for (int i = 0; i < getParityBits(); i++) {
		for (int j = 0; j < getParityBits(); j++) {
			if (i == j) {
				H_aux[i][j] = 1;
			} else {
				H_aux[i][j] = 0;
			}
		}
	}

	/* Data Part of G matrix */
	for (int i = getParityBits(); i < hamming_n; i++) {
		for (int j = 0; j < getParityBits(); j++) {
			if (i - getParityBits() == j) {
				H_aux[i][j] = 0;
			} else {
				H_aux[i][j] = 1;
			}
		}
	}

	/* Assign the auxiliary matrix to the private G matrix */
	H = H_aux;
}

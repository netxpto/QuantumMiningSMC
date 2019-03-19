#include <algorithm>
#include <iostream>
#include <Windows.h>

#include "netxpto_20180418.h"
#include "hamming_encoder_20180608.h"

using namespace std;

void HammingCoder::initialize(void) {
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
}

bool HammingCoder::runBlock(void) {
	bool alive{ false };

	/* Update the parity bits according to the defined nBits and kBits */
	setParityBits();
	if (parityBits == 0) {
		OutputDebugString("ERROR: (Hamming Coder) Invalid conmbination of (n, k) Hamming Code. \n");

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
		std::vector<int> inputData(hamming_k);
		std::vector<int> outputData(hamming_n);

		// Get the first k bits
		for (int k = 0; k < hamming_k; k++) {
			int in;
			inputSignals[0]->bufferGet(&in);

			inputData[k] = in;
		}

		// Multiply Vector by Matrix
		for (int k = 0; k < hamming_n; k++) {
			for (int m = 0; m < hamming_k; m++) {
				outputData[k] += (G[k][m] * inputData[m]);

				if (outputData[k] > 1) {
					outputData[k] = 0;
				}
			}
		}

		// Store data in output
		for (int k = 0; k < hamming_n; k++) {
			outputSignals[0]->bufferPut(outputData[k]);
		}

		ready = inputSignals[0]->ready();
		space = outputSignals[0]->space();
	}

	return alive;
}

void HammingCoder::setParityBits(void) {
	/* Save the selected parity bits for the Hamming Code */
	parityBits = getNBits() - getKBits();

	for (int k = 0; k < (int)size(parity_array); k++) {
		if (parity_array[k] == parityBits) {
			if ((k_array[k] == kBits) && (n_array[k] == nBits)) {
				/* Calculate the G matrix according to the selected parity bits */
				setGMatrix(parityBits);
				return;
			}
		}
	}

	parityBits = 0;
}

int HammingCoder::getNBits() {
	return nBits;
}

void HammingCoder::setNBits(int s_n) {
	/* Save the selected Code Word length for the Hamming Code */
	nBits = s_n;
}

int HammingCoder::getKBits() {
	return kBits;
}

void HammingCoder::setKBits(int s_n) {
	/* Save the selected Data length for the Hamming Code */
	kBits = s_n;
}

int HammingCoder::getParityBits() {
	return parityBits;
}

void HammingCoder::setGMatrix(int s_n) {
	/* Determine n and k parameters according to selected parity for Hamming Code */
	const int hamming_n = n_array[getParityBits() - 2];
	const int hamming_k = k_array[getParityBits() - 2];

	/* Auxiliary G Matrix to perform calculations */
	vector<vector<int>> G_aux(hamming_n, vector<int>(hamming_k));

	/* Parity Part of G matrix */
	for (int i = 0; i < getParityBits(); i++) {
		for (int j = 0; j < hamming_k; j++) {
			if (i == j) {
				G_aux[i][j] = 0;
			} else {
				G_aux[i][j] = 1;
			}
		}
	}

	/* Data Part of G matrix */
	for (int i = getParityBits(); i < hamming_n; i++) {
		for (int j = 0; j < hamming_k; j++) {
			if (i - getParityBits() == j) {
				G_aux[i][j] = 1;
			} else {
				G_aux[i][j] = 0;
			}
		}
	}

	/* Assign the auxiliary matrix to the private G matrix */
	G = G_aux;
}

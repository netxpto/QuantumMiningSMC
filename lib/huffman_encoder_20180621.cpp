#include <algorithm>
#include <iostream>
#include <Windows.h>

#include "netxpto_20180418.h"
#include "huffman_encoder_20180621.h"

using namespace std;

void HuffmanEncoder::initialize(void) {
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
}

bool HuffmanEncoder::runBlock(void) {
	bool alive{ false };



	/* Avaiable bits on input buffer */
 	int ready = inputSignals[0]->ready();

	/* Avaiable space on output buffer */
	int space = outputSignals[0]->space();

	int saveOrder = getSourceOrder();
	int spaceLimit;

	if (saveOrder == 2) {
		spaceLimit = 3;
	}
	else if (saveOrder == 3) {
		spaceLimit = 7;
	}
	else {
		spaceLimit = 15;
	}

	/* Cycle to process data */
	while ((ready >= saveOrder) && (space >= spaceLimit)) {
		int a1, a2, a3, a4;

		switch (sourceOrder) {
		case 2:
			inputSignals[0]->bufferGet(&a1);
			inputSignals[0]->bufferGet(&a2);

			if (probabilityOfZero > 0.5) {
				if ((a1 == 1) && (a2 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);

				}
				else if ((a1 == 0) && (a2 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);

				}
				else if ((a1 == 1) && (a2 == 0)) {
					int out0 = 1;
					int out1 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);

				}
				else {
					int out = 0;

					outputSignals[0]->bufferPut((t_binary)out);
				}
			}
			else {
				if ((a1 == 0) && (a2 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);

				}
				else if ((a1 == 1) && (a2 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);

				}
				else if ((a1 == 0) && (a2 == 1)) {
					int out0 = 1;
					int out1 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);

				}
				else {
					int out = 0;

					outputSignals[0]->bufferPut((t_binary)out);
				}
			}

			break;
		case 3:
			inputSignals[0]->bufferGet(&a1);
			inputSignals[0]->bufferGet(&a2);
			inputSignals[0]->bufferGet(&a3);

			if (probabilityOfZero > 0.5) {
				if ((a1 == 1) && (a2 == 1) && (a3 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
				}
				else if ((a1 == 0) && (a2 == 1) && (a3 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);

				}
				else if ((a1 == 1) && (a2 == 0) && (a3 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);

				}
				else if ((a1 == 0) && (a2 == 0) && (a3 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);

				}
				else if ((a1 == 1) && (a2 == 1) && (a3 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);

				}
				else if ((a1 == 0) && (a2 == 1) && (a3 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);

				}
				else if ((a1 == 1) && (a2 == 0) && (a3 == 0)) {
					int out0 = 1;
					int out1 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);

				}
				else {
					int out = 0;

					outputSignals[0]->bufferPut((t_binary)out);
				}
			}
			else {
				if ((a1 == 0) && (a2 == 0) && (a3 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
				}
				else if ((a1 == 1) && (a2 == 0) && (a3 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);

				}
				else if ((a1 == 0) && (a2 == 1) && (a3 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);

				}
				else if ((a1 == 1) && (a2 == 1) && (a3 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);

				}
				else if ((a1 == 0) && (a2 == 0) && (a3 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);

				}
				else if ((a1 == 1) && (a2 == 0) && (a3 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);

				}
				else if ((a1 == 0) && (a2 == 1) && (a3 == 1)) {
					int out0 = 1;
					int out1 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);

				}
				else {
					int out = 0;

					outputSignals[0]->bufferPut((t_binary)out);
				}
			}

			break;
		case 4:
			inputSignals[0]->bufferGet(&a1);
			inputSignals[0]->bufferGet(&a2);
			inputSignals[0]->bufferGet(&a3);
			inputSignals[0]->bufferGet(&a4);

			if (probabilityOfZero > 0.5) {
				if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;
					int out7 = 1;
					int out8 = 1;
					int out9 = 1;
					int out10 = 1;
					int out11 = 1;
					int out12 = 1;
					int out13 = 1;
					int out14 = 1;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
					outputSignals[0]->bufferPut((t_binary)out7);
					outputSignals[0]->bufferPut((t_binary)out8);
					outputSignals[0]->bufferPut((t_binary)out9);
					outputSignals[0]->bufferPut((t_binary)out10);
					outputSignals[0]->bufferPut((t_binary)out11);
					outputSignals[0]->bufferPut((t_binary)out12);
					outputSignals[0]->bufferPut((t_binary)out13);
					outputSignals[0]->bufferPut((t_binary)out14);
				}
				else if ((a1 == 0) && (a2 == 1) && (a3 == 1) && (a4 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;
					int out7 = 1;
					int out8 = 1;
					int out9 = 1;
					int out10 = 1;
					int out11 = 1;
					int out12 = 1;
					int out13 = 1;
					int out14 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
					outputSignals[0]->bufferPut((t_binary)out7);
					outputSignals[0]->bufferPut((t_binary)out8);
					outputSignals[0]->bufferPut((t_binary)out9);
					outputSignals[0]->bufferPut((t_binary)out10);
					outputSignals[0]->bufferPut((t_binary)out11);
					outputSignals[0]->bufferPut((t_binary)out12);
					outputSignals[0]->bufferPut((t_binary)out13);
					outputSignals[0]->bufferPut((t_binary)out14);

				}
				else if ((a1 == 1) && (a2 == 0) && (a3 == 1) && (a4 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;
					int out7 = 1;
					int out8 = 1;
					int out9 = 1;
					int out10 = 1;
					int out11 = 1;
					int out12 = 1;
					int out13 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
					outputSignals[0]->bufferPut((t_binary)out7);
					outputSignals[0]->bufferPut((t_binary)out8);
					outputSignals[0]->bufferPut((t_binary)out9);
					outputSignals[0]->bufferPut((t_binary)out10);
					outputSignals[0]->bufferPut((t_binary)out11);
					outputSignals[0]->bufferPut((t_binary)out12);
					outputSignals[0]->bufferPut((t_binary)out13);

				}
				else if ((a1 == 0) && (a2 == 0) && (a3 == 1) && (a4 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;
					int out7 = 1;
					int out8 = 1;
					int out9 = 1;
					int out10 = 1;
					int out11 = 1;
					int out12 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
					outputSignals[0]->bufferPut((t_binary)out7);
					outputSignals[0]->bufferPut((t_binary)out8);
					outputSignals[0]->bufferPut((t_binary)out9);
					outputSignals[0]->bufferPut((t_binary)out10);
					outputSignals[0]->bufferPut((t_binary)out11);
					outputSignals[0]->bufferPut((t_binary)out12);

				}
				else if ((a1 == 1) && (a2 == 1) && (a3 == 0) && (a4 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;
					int out7 = 1;
					int out8 = 1;
					int out9 = 1;
					int out10 = 1;
					int out11 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
					outputSignals[0]->bufferPut((t_binary)out7);
					outputSignals[0]->bufferPut((t_binary)out8);
					outputSignals[0]->bufferPut((t_binary)out9);
					outputSignals[0]->bufferPut((t_binary)out10);
					outputSignals[0]->bufferPut((t_binary)out11);

				}
				else if ((a1 == 0) && (a2 == 1) && (a3 == 0) && (a4 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;
					int out7 = 1;
					int out8 = 1;
					int out9 = 1;
					int out10 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
					outputSignals[0]->bufferPut((t_binary)out7);
					outputSignals[0]->bufferPut((t_binary)out8);
					outputSignals[0]->bufferPut((t_binary)out9);
					outputSignals[0]->bufferPut((t_binary)out10);
				}
				else if ((a1 == 1) && (a2 == 0) && (a3 == 0) && (a4 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;
					int out7 = 1;
					int out8 = 1;
					int out9 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
					outputSignals[0]->bufferPut((t_binary)out7);
					outputSignals[0]->bufferPut((t_binary)out8);
					outputSignals[0]->bufferPut((t_binary)out9);

				}
				else if ((a1 == 0) && (a2 == 0) && (a3 == 0) && (a4 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;
					int out7 = 1;
					int out8 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
					outputSignals[0]->bufferPut((t_binary)out7);
					outputSignals[0]->bufferPut((t_binary)out8);

				}
				else if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;
					int out7 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
					outputSignals[0]->bufferPut((t_binary)out7);

				}
				else if ((a1 == 0) && (a2 == 1) && (a3 == 1) && (a4 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);

				}
				else if ((a1 == 1) && (a2 == 0) && (a3 == 1) && (a4 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);

				}
				else if ((a1 == 0) && (a2 == 0) && (a3 == 1) && (a4 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);

				}
				else if ((a1 == 1) && (a2 == 1) && (a3 == 0) && (a4 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);

				}
				else if ((a1 == 0) && (a2 == 1) && (a3 == 0) && (a4 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
				}
				else if ((a1 == 1) && (a2 == 0) && (a3 == 0) && (a4 == 0)) {
					int out0 = 1;
					int out1 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
				}
				else {
					int out0 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
				}
			}
			else {
				if ((a1 == 0) && (a2 == 0) && (a3 == 0) && (a4 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;
					int out7 = 1;
					int out8 = 1;
					int out9 = 1;
					int out10 = 1;
					int out11 = 1;
					int out12 = 1;
					int out13 = 1;
					int out14 = 1;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
					outputSignals[0]->bufferPut((t_binary)out7);
					outputSignals[0]->bufferPut((t_binary)out8);
					outputSignals[0]->bufferPut((t_binary)out9);
					outputSignals[0]->bufferPut((t_binary)out10);
					outputSignals[0]->bufferPut((t_binary)out11);
					outputSignals[0]->bufferPut((t_binary)out12);
					outputSignals[0]->bufferPut((t_binary)out13);
					outputSignals[0]->bufferPut((t_binary)out14);
				}
				else if ((a1 == 1) && (a2 == 0) && (a3 == 0) && (a4 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;
					int out7 = 1;
					int out8 = 1;
					int out9 = 1;
					int out10 = 1;
					int out11 = 1;
					int out12 = 1;
					int out13 = 1;
					int out14 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
					outputSignals[0]->bufferPut((t_binary)out7);
					outputSignals[0]->bufferPut((t_binary)out8);
					outputSignals[0]->bufferPut((t_binary)out9);
					outputSignals[0]->bufferPut((t_binary)out10);
					outputSignals[0]->bufferPut((t_binary)out11);
					outputSignals[0]->bufferPut((t_binary)out12);
					outputSignals[0]->bufferPut((t_binary)out13);
					outputSignals[0]->bufferPut((t_binary)out14);

				}
				else if ((a1 == 0) && (a2 == 1) && (a3 == 0) && (a4 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;
					int out7 = 1;
					int out8 = 1;
					int out9 = 1;
					int out10 = 1;
					int out11 = 1;
					int out12 = 1;
					int out13 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
					outputSignals[0]->bufferPut((t_binary)out7);
					outputSignals[0]->bufferPut((t_binary)out8);
					outputSignals[0]->bufferPut((t_binary)out9);
					outputSignals[0]->bufferPut((t_binary)out10);
					outputSignals[0]->bufferPut((t_binary)out11);
					outputSignals[0]->bufferPut((t_binary)out12);
					outputSignals[0]->bufferPut((t_binary)out13);

				}
				else if ((a1 == 1) && (a2 == 1) && (a3 == 0) && (a4 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;
					int out7 = 1;
					int out8 = 1;
					int out9 = 1;
					int out10 = 1;
					int out11 = 1;
					int out12 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
					outputSignals[0]->bufferPut((t_binary)out7);
					outputSignals[0]->bufferPut((t_binary)out8);
					outputSignals[0]->bufferPut((t_binary)out9);
					outputSignals[0]->bufferPut((t_binary)out10);
					outputSignals[0]->bufferPut((t_binary)out11);
					outputSignals[0]->bufferPut((t_binary)out12);

				}
				else if ((a1 == 0) && (a2 == 0) && (a3 == 1) && (a4 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;
					int out7 = 1;
					int out8 = 1;
					int out9 = 1;
					int out10 = 1;
					int out11 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
					outputSignals[0]->bufferPut((t_binary)out7);
					outputSignals[0]->bufferPut((t_binary)out8);
					outputSignals[0]->bufferPut((t_binary)out9);
					outputSignals[0]->bufferPut((t_binary)out10);
					outputSignals[0]->bufferPut((t_binary)out11);

				}
				else if ((a1 == 1) && (a2 == 0) && (a3 == 1) && (a4 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;
					int out7 = 1;
					int out8 = 1;
					int out9 = 1;
					int out10 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
					outputSignals[0]->bufferPut((t_binary)out7);
					outputSignals[0]->bufferPut((t_binary)out8);
					outputSignals[0]->bufferPut((t_binary)out9);
					outputSignals[0]->bufferPut((t_binary)out10);
				}
				else if ((a1 == 0) && (a2 == 1) && (a3 == 1) && (a4 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;
					int out7 = 1;
					int out8 = 1;
					int out9 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
					outputSignals[0]->bufferPut((t_binary)out7);
					outputSignals[0]->bufferPut((t_binary)out8);
					outputSignals[0]->bufferPut((t_binary)out9);

				}
				else if ((a1 == 1) && (a2 == 1) && (a3 == 1) && (a4 == 0)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;
					int out7 = 1;
					int out8 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
					outputSignals[0]->bufferPut((t_binary)out7);
					outputSignals[0]->bufferPut((t_binary)out8);

				}
				else if ((a1 == 0) && (a2 == 0) && (a3 == 0) && (a4 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 1;
					int out7 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);
					outputSignals[0]->bufferPut((t_binary)out7);

				}
				else if ((a1 == 1) && (a2 == 0) && (a3 == 0) && (a4 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 1;
					int out6 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);
					outputSignals[0]->bufferPut((t_binary)out6);

				}
				else if ((a1 == 0) && (a2 == 1) && (a3 == 0) && (a4 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 1;
					int out5 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);
					outputSignals[0]->bufferPut((t_binary)out5);

				}
				else if ((a1 == 1) && (a2 == 1) && (a3 == 0) && (a4 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 1;
					int out4 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);
					outputSignals[0]->bufferPut((t_binary)out4);

				}
				else if ((a1 == 0) && (a2 == 0) && (a3 == 1) && (a4 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 1;
					int out3 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
					outputSignals[0]->bufferPut((t_binary)out3);

				}
				else if ((a1 == 1) && (a2 == 0) && (a3 == 1) && (a4 == 1)) {
					int out0 = 1;
					int out1 = 1;
					int out2 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
					outputSignals[0]->bufferPut((t_binary)out2);
				}
				else if ((a1 == 0) && (a2 == 1) && (a3 == 1) && (a4 == 1)) {
					int out0 = 1;
					int out1 = 0;

					outputSignals[0]->bufferPut((t_binary)out0);
					outputSignals[0]->bufferPut((t_binary)out1);
				}
				else {
					int out = 0;

					outputSignals[0]->bufferPut((t_binary)out);
				}
			}
			break;
		default:
			return alive;
			break;
		}

		ready = inputSignals[0]->ready();
		space = outputSignals[0]->space();
	}

	return alive;
}

double HuffmanEncoder::getProbabilityOfZero() {
	return probabilityOfZero;
}

void HuffmanEncoder::setProbabilityOfZero(double s_n) {
	probabilityOfZero = s_n;
}

int HuffmanEncoder::getSourceOrder() {
	return sourceOrder;
}

void HuffmanEncoder::setSourceOrder(int s_n) {
	sourceOrder = s_n;
}
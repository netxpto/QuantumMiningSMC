#include <algorithm>
#include <iostream>
#include <Windows.h>

#include "netxpto_20180418.h"
#include "source_code_efficiency_20180621.h"

using namespace std;

void SourceCodeEfficiency::initialize(void) {
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
}

bool SourceCodeEfficiency::runBlock(void) {
	bool alive{ false };

	/* Avaiable bits on input buffer */
	int ready = inputSignals[0]->ready();

	/* Avaiable space on output buffer */
	int space = outputSignals[0]->space();

	/* Cycle to process data */
	while ((ready > 0) && (space > 0)) {
		int counter = 0;
		bool symbolDetected = false;

		while (symbolDetected == false) {
			symbolDetected = getData(counter);
			counter++;
		}

		ready = inputSignals[0]->ready();
		space = outputSignals[0]->space();
	}

	return alive;
}

double SourceCodeEfficiency::getProbabilityOfZero() {
	return probabilityOfZero;
}

void SourceCodeEfficiency::setProbabilityOfZero(double s_n) {
	probabilityOfZero = s_n;
}

int SourceCodeEfficiency::getSourceOrder() {
	return sourceOrder;
}

void SourceCodeEfficiency::setSourceOrder(int s_n) {
	sourceOrder = s_n;
}

bool SourceCodeEfficiency::getData(int counter) {
	int in;
	inputSignals[0]->bufferGet(&in);

	if (sourceOrder == 2) {
		if ((counter == 0) && (in == 0)) {
			counter_X2[3] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		} else if ((counter == 1) && (in == 0)) {
			counter_X2[2] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		} else if ((counter == 2) && (in == 0)) {
			counter_X2[1] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		} else if( (counter == 2) && (in == 1) ){
			counter_X2[0] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		} 
	}

	if (sourceOrder == 3) {
		if ((counter == 0) && (in == 0)) {
			counter_X3[7] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 1) && (in == 0)) {
			counter_X3[6] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 2) && (in == 0)) {
			counter_X3[5] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 3) && (in == 0)) {
			counter_X3[4] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 4) && (in == 0)) {
			counter_X3[3] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 5) && (in == 0)) {
			counter_X3[2] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 6) && (in == 0)) {
			counter_X3[1] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 6) && (in == 1)) {
			counter_X3[0] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
	}

	if (sourceOrder == 4) {
		if ((counter == 0) && (in == 0)) {
			counter_X4[15] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 1) && (in == 0)) {
			counter_X4[14] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 2) && (in == 0)) {
			counter_X4[13] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 3) && (in == 0)) {
			counter_X4[12] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 4) && (in == 0)) {
			counter_X4[11] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 5) && (in == 0)) {
			counter_X4[10] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 6) && (in == 0)) {
			counter_X4[9] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 7) && (in == 0)) {
			counter_X4[8] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 8) && (in == 0)) {
			counter_X4[7] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 9) && (in == 0)) {
			counter_X4[6] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 10) && (in == 0)) {
			counter_X4[5] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 11) && (in == 0)) {
			counter_X4[4] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 12) && (in == 0)) {
			counter_X4[3] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 13) && (in == 0)) {
			counter_X4[2] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 14) && (in == 0)) {
			counter_X4[1] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
		else if ((counter == 14) && (in == 1)) {
			counter_X4[0] += 1;
			totalSymbols += 1;

			efficiencyCalc();

			return true;

		}
	}

	return false;
}

void SourceCodeEfficiency::efficiencyCalc(void) {
	double sumEntropy = 0;
	double sumLength = 0;
	double efficiency = 0;

	switch (sourceOrder) {
	case 2:
		for (int i = 0; i < 4; i++) {
			probability_X2[i] = counter_X2[i] / (double)totalSymbols;
			length_X2[i] = probability_X2[i] * symbolLength_X2[i];

			if (probability_X2[i] != 0) {
				entropy_X2[i] = probability_X2[i] * log2(1 / probability_X2[i]);
			}
		}

		for (int i = 0; i < 4; i++) {
			sumEntropy += entropy_X2[i];
			sumLength += length_X2[i];
		}

		efficiency = (sumEntropy / sumLength) * 100;
		outputSignals[0]->bufferPut((t_real)efficiency);
		break;

	case 3:
		for (int i = 0; i < 8; i++) {
			probability_X3[i] = counter_X3[i] / (double)totalSymbols;
			length_X3[i] = probability_X3[i] * symbolLength_X3[i];

			if (probability_X3[i] != 0) {
				entropy_X3[i] = probability_X3[i] * log2(1 / probability_X3[i]);
			}
		}

		for (int i = 0; i < 8; i++) {
			sumEntropy += entropy_X3[i];
			sumLength += length_X3[i];
		}

		efficiency = (sumEntropy / sumLength) * 100;
		outputSignals[0]->bufferPut((t_real) efficiency);
		break;

	case 4:
		for (int i = 0; i < 16; i++) {
			probability_X4[i] = counter_X4[i] / (double)totalSymbols;
			length_X4[i] = probability_X4[i] * symbolLength_X4[i];

			if (probability_X4[i] != 0) {
				entropy_X4[i] = probability_X4[i] * log2(1 / probability_X4[i]);
			}
		}

		for (int i = 0; i < 16; i++) {
			sumEntropy += entropy_X4[i];
			sumLength += length_X4[i];
		}

		efficiency = (sumEntropy / sumLength) * 100;
		outputSignals[0]->bufferPut((t_real) efficiency);
		break;

	default:
		break;
	}
}


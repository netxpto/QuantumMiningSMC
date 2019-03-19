#include "../include/electronic_polarization_controller_20181218.h"

void ElectronicPolarizationController::initialize(void) {
	for (auto k : outputSignals)
	{
		k->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
		k->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
		k->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	}
	input1.real(0); input1.imag(0);
	input2.real(0); input2.imag(0);
	input3.real(0); input3.imag(0);
	input4.real(0); input4.imag(0);
	for (auto i = 0; i < 2; i++) {
		for (auto j = 0; j < 2; j++) {
			if (i == j)
				rotMatrix[i][j] = 1;
			else
				rotMatrix[i][j] = 0;
		}
	}
}

bool ElectronicPolarizationController::runBlock(void) {
	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();

	int ready = min(ready0, ready1);

	auto space = MAX_BUFFER_LENGTH;
	for (auto k : outputSignals) space = min(space, k->space());

	int process = min(ready, space);

	if (process <= 0) return false;

	for (auto k = 0; k < process; k++) {

		t_complex_xy valueXY;
		t_complex dIn1{}, dIn2{}, dIn3{}, dIn4{};

		inputSignals[0]->bufferGet(&valueXY);
		inputSignals[1]->bufferGet(&dIn1);
		inputSignals[2]->bufferGet(&dIn2);
		inputSignals[3]->bufferGet(&dIn3);
		inputSignals[4]->bufferGet(&dIn4);

		t_complex valueX = valueXY.x;
		t_complex valueY = valueXY.y;
		/* Actualize rotation matrix */
		if (((dIn1 != input1) || (dIn2 != input2) || (dIn3 != input3) || (dIn4 != input4)) && (abs(valueX)!=0 && abs(valueY)!=0)) {
			t_complex rotMatrixNew[2][2] = { {dIn1, dIn2},{dIn3, dIn4} };
			t_complex result[2][2] = { {0,0},{0,0} };

			for (auto i = 0; i < 2; i++)
				for (auto j = 0; j < 2; j++) {
					for (auto k = 0; k < 2; k++) {
						result[i][j] += rotMatrixNew[i][k] * rotMatrix[k][j];
					}
				}
			for (auto i = 0; i < 2; i++)
				for (auto j = 0; j < 2; j++)
					rotMatrix[i][j] = result[i][j];

			input1 = dIn1; input2 = dIn2; input3 = dIn3; input4 = dIn4;
		}
		/* Rotation */
		t_complex JvIn[2][1] = { {valueX},{valueY} };
		t_complex JvOut[2][1] = { {0},{0} };

		for (auto i = 0; i < 2; i++) {
			for (auto j = 0; j < 1; j++) {
				for (auto k = 0; k < 2; k++) {
					JvOut[i][j] += rotMatrix[i][k] * JvIn[k][j];
				}
			}
		}
		t_complex valueXout = JvOut[0][0];
		t_complex valueYout = JvOut[1][0];

		/*Jones To Stokes*/
		t_complex valueX_conj = conj(valueXout);
		t_complex valueY_conj = conj(valueYout);

		t_real S0 = real(valueXout*valueX_conj + valueYout * valueY_conj);
		t_real S1 = real(valueXout*valueX_conj - valueYout * valueY_conj);
		t_real S2 = real(valueXout*valueY_conj + valueYout * valueX_conj);
		t_real S3 = real(1i*(valueXout*valueY_conj - valueX_conj * valueYout));

		/*Output result state*/
		t_complex_xy valueXYout = { valueXout, valueYout };
		
		outputSignals[0]->bufferPut((t_complex_xy)valueXYout);

	}

	return true;
}


#include "../include/polarization_rotator_20181113.h"

void PolarizationRotator::initialize(void) {
	for (auto k : outputSignals)
	{
		k->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
		k->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
		k->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	}
	
}

bool PolarizationRotator::runBlock(void) {
	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();

	int ready = min(ready0, ready1);

	auto space = MAX_BUFFER_LENGTH;
	for (auto k : outputSignals) space = min(space, k->space());

	int process = min(ready, space);

	if (process <= 0) return false;

	for (auto k = 0; k < process; k++) {

		t_complex_xy valueXY;
		t_real dTheta{};
		t_real dPhi{};

		inputSignals[0]->bufferGet(&valueXY);
		inputSignals[1]->bufferGet(&dTheta);

		if (numberOfInputSignals == 3)
			inputSignals[2]->bufferGet(&dPhi);

		t_complex valueX = valueXY.x;
		t_complex valueY = valueXY.y;

		t_complex valueX_conj = conj(valueX);
		t_complex valueY_conj = conj(valueY);

		if (((dTheta != theta) && (dTheta != 0.0)) || ((dPhi != phi) && (dPhi != 0))) {
			double results_aux[3][3] = { {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0} };
			double r1[3][3] = { {1,0,0},{0,cos(dPhi),-sin(dPhi)},{0,sin(dPhi),cos(dPhi)} };
			double r2[3][3] = { {cos(dPhi),0,sin(dPhi)},{0,1,0},{-sin(dPhi),0,cos(dPhi)} };
			double r3[3][3] = { {cos(dTheta),-sin(dTheta),0},{sin(dTheta),cos(dTheta),0},{0,0,1} };

			double results[3][3] = { {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0} };
			double results_aux2[3][3] = { {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0} };
			if (n > 0) {
				/*rotation matrix = r3*r2*r1*/
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++) {
						for (int u = 0; u < 3; u++)
							results_aux[i][j] += r3[i][u] * r2[u][j];
					}

				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++) {
						for (int u = 0; u < 3; u++)
							results_aux2[i][j] += results_aux[i][u] * r1[u][j];
					}

				for (auto i = 0; i < 3; i++)
					for (auto j = 0; j < 3; j++) {
						for (auto u = 0; u < 3; u++)
							results[i][j] += rotation[i][u] * results_aux2[u][j];
					}
			}
			else {
				/*rotation matrix = r1*r2*r3*/
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++) {
						for (int u = 0; u < 3; u++)
							results_aux[i][j] += r1[i][u] * r2[u][j];
					}

				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++) {
						for (int u = 0; u < 3; u++)
							results_aux2[i][j] += results_aux[i][u] * r3[u][j];
					}

				for (auto i = 0; i < 3; i++)
					for (auto j = 0; j < 3; j++) {
						for (auto u = 0; u < 3; u++)
							results[i][j] += rotation[i][u] * results_aux2[u][j];
					}
				n++;
			}
			
			for (auto i = 0; i < 3; i++)
				for (auto j = 0; j < 3; j++) {
					rotation[i][j] = results[i][j];
				}

			theta = dTheta;
			phi = dPhi;
		}

		/*Jones To Stokes*/
		t_real S0 = real(valueX*valueX_conj + valueY * valueY_conj);
		t_real S1 = real(valueX*valueX_conj - valueY * valueY_conj);
		t_real S2 = real(valueX*valueY_conj + valueY * valueX_conj);
		t_real S3 = real(1i*(-valueX*valueY_conj + valueX_conj * valueY));

		t_complex valueXout;
		t_complex valueYout;
		if (S0 == 0) {
			valueXout = valueX;
			valueYout = valueY;
		}
		else{

			t_real S1_out = S1*rotation[0][0] + S2*rotation[1][0] + S3*rotation[2][0];
			t_real S2_out = S1*rotation[0][1] + S2*rotation[1][1] + S3*rotation[2][1];
			t_real S3_out = S1*rotation[0][2] + S2*rotation[1][2] + S3*rotation[2][2];

			/*Stokes To jones*/
			//Degree of polarization
			double p = sqrt((double)(pow(S1_out, 2) + pow(S2_out, 2) + pow(S3_out, 2))) / S0;
			/*Normalize the Stokes parameters(first one will be 1, of course)*/
			double Q = S1_out / (S0*p);
			double U = S2_out / (S0*p);
			double V = S3_out / (S0*p);
			/*And construct the 2 Jones components*/
			double A = sqrt((1 + Q) / 2);
			t_complex B{};
			if (A == 0)
				B = 1;
			else
				B = U / (2 * A) - 1i*V / (2 * A);

			valueXout = (t_complex)A* sqrt(S0*p);
			valueYout = (t_complex)B* sqrt(S0*p);
		}

		t_complex_xy valueXYout = { valueXout, valueYout };
		
		outputSignals[0]->bufferPut((t_complex_xy)valueXYout);

	}

	return true;
}


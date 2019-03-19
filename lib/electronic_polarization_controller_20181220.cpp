#include "../include/electronic_polarization_controller_20181220.h"

void ElectronicPolarizationController::initialize(void) {
	for (auto k : outputSignals) {
		k->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
		k->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
		k->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	}
}

bool ElectronicPolarizationController::runBlock(void) {

	int ready = MAX_BUFFER_LENGTH;
	for (auto k : inputSignals) ready = min(ready, k->ready());

	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process <= 0) return false;

	for (auto k = 0; k < process; k++) {
		t_complex_xy inPhoton;
		inputSignals[0]->bufferGet(&inPhoton);

		double dTheta1, dTheta2, dTheta3;
		inputSignals[1]->bufferGet(&dTheta1);
		inputSignals[2]->bufferGet(&dTheta2);
		inputSignals[3]->bufferGet(&dTheta3);

		if (firstTime) {
			actualizeMatrixF(matrixF, theta1, theta2, theta3);
			firstTime = false;
		}
		if ((dTheta1 == -5) && (dTheta2 == -5) && (dTheta3 == -5)) {
			double previousMinverted[3][3] = { {0,0,0},{0,0,0},{0,0,0} };
			inverse(previousM, previousMinverted);
			double mult[3][3] = { {0,0,0} ,{0,0,0} ,{0,0,0} };
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++) {
					for (int u = 0; u < 3; u++)
						mult[i][j] += previousMinverted[i][u] * matrixF[u][j];
				}
			for (auto i = 0; i < 3; i++)
				for (auto j = 0; j < 3; j++) {
					matrixF[i][j] = mult[i][j];
				}
			theta1 = dTheta1; theta2 = dTheta2; theta3 = dTheta3;
			dTheta1 = 0; dTheta2 = 0; dTheta3 = 0;
		}
		else {
			if ((dTheta1 != theta1 && dTheta1 != 0.0 &&  dTheta1 != -5) || (dTheta2 != theta2 && dTheta2 != 0.0 &&  dTheta2 != -5) || (dTheta3 != theta3 && dTheta3 != 0.0 &&  dTheta3 != -5)) {
				double aux[3][3] = { {0,0,0},{0,0,0},{0,0,0} };
				double mult[3][3] = { {0,0,0} ,{0,0,0} ,{0,0,0} };
				actualizeMatrixF(aux, dTheta1, dTheta2, dTheta3);
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++) {
						for (int u = 0; u < 3; u++)
							mult[i][j] += aux[i][u] * matrixF[u][j];
					}
				for (auto i = 0; i < 3; i++)
					for (auto j = 0; j < 3; j++) {
						matrixF[i][j] = mult[i][j];
						previousM[i][j] = aux[i][j];
					}

				theta1 = dTheta1; theta2 = dTheta2; theta3 = dTheta3;
			}
		}
		
		

		array<double, 4> stokesP = jones2stokes(inPhoton);
		double si[3] = { {stokesP[1]},{stokesP[2]},{stokesP[3]} };
		double so[3] = { 0,0,0};
		for(auto i = 0; i < 3; i++)
			for (auto j = 0; j < 3; j++) {
				so[i] += matrixF[i][j] * si[j];
			}
		double S0 = sqrt(pow(so[0], 2) + pow(so[1], 2) + pow(so[2], 2));
		if (S0 > 0) {
			array<double, 4>stokesPOut = { S0,so[0],so[1],so[2] };
			double gamma = acos(so[0]);
			t_complex_xy photonOut = stokes2jones(stokesPOut);
			outputSignals[0]->bufferPut(photonOut);

		}
		else
			outputSignals[0]->bufferPut(inPhoton);

	}
	return true;
}

void ElectronicPolarizationController::actualizeR(double(&Matrix)[3][3], double theta) {
	Matrix[0][0] = cos(2 * theta);
	Matrix[0][1] = -sin(2 * theta);
	Matrix[0][2] = 0;
	Matrix[1][0] = sin(2 * theta);
	Matrix[1][1] = cos(2 * theta);
	Matrix[1][2] = 0;
	Matrix[2][0] = 0;
	Matrix[2][1] = 0;
	Matrix[2][2] = 1;
}

void ElectronicPolarizationController::matrixEPCPlate(double(&M)[3][3], double(&Mlambda)[3][3], double angle) {
	double R1[3][3] = { {0,0,0} ,{0,0,0} ,{0,0,0} };
	double R[3][3] = { {0,0,0} ,{0,0,0} ,{0,0,0} };
	actualizeR(R1, -angle);
	actualizeR(R, angle);

	double mult1[3][3] = { {0,0,0} , {0,0,0} , {0,0,0} };
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++) {
			for (int u = 0; u < 3; u++)
				mult1[i][j] += Mlambda[i][u] * R1[u][j];
		}
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++) {
			for (int u = 0; u < 3; u++)
				M[i][j] += R[i][u] * mult1[u][j];
		}
}

void ElectronicPolarizationController::actualizeMatrixF(double(&F)[3][3], double t1, double t2, double t3) {
	double firstPlate[3][3] = { {0,0,0} ,{0,0,0} ,{0,0,0} };
	double secondPlate[3][3] = { {0,0,0} ,{0,0,0} ,{0,0,0} };
	double thirdPlate[3][3] = { {0,0,0} ,{0,0,0} ,{0,0,0} };
	double mult_aux[3][3] = { {0,0,0},{0,0,0},{0,0,0} };
	matrixEPCPlate(firstPlate, mLambda4, t1);
	matrixEPCPlate(secondPlate, mLambda2, t2);
	matrixEPCPlate(thirdPlate, mLambda4, t3);

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++) {
			for (int u = 0; u < 3; u++)
				mult_aux[i][j] += secondPlate[i][u] * firstPlate[u][j];
		}

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++) {
			for (int u = 0; u < 3; u++)
				F[i][j] += thirdPlate[i][u] * mult_aux[u][j];
		}
}

array <double, 4> ElectronicPolarizationController::jones2stokes(t_complex_xy photon) {
	t_complex Ax = photon.x;
	t_complex Ay = photon.y;

	t_complex Ax_conj = conj(Ax);
	t_complex Ay_conj = conj(Ay);

	/*Jones To Stokes*/
	t_real S0 = real(Ax*Ax_conj + Ay * Ay_conj);
	t_real S1 = real(Ax*Ax_conj - Ay * Ay_conj);
	t_real S2 = real(Ax*Ay_conj + Ay * Ax_conj);
	t_real S3 = real(1i*(-Ax * Ay_conj + Ax_conj * Ay));

	if ((S0 <= 0) && (S1>0) && (S2 > 0) && (S3 > 0))
		cout << "ERROR: EPC - jones2stokes \n";

	array <double, 4> stokesParameters = { S0,S1,S2,S3 };
	return stokesParameters;
}

t_complex_xy ElectronicPolarizationController::stokes2jones(array<double, 4>sParameters) {
	//Degree of polarization
	double p = sqrt((double)(pow(sParameters[1], 2) + pow(sParameters[2], 2) + pow(sParameters[3], 2))) / sParameters[0];
	/*Normalize the Stokes parameters(first one will be 1, of course)*/
	double Q = sParameters[1] / (sParameters[0] *p);
	double U = sParameters[2] / (sParameters[0] *p);
	double V = sParameters[3] / (sParameters[0] *p);
	/*And construct the 2 Jones components*/
	double A = sqrt((1 + Q) / 2);
	t_complex B{};
	if (A == 0)
		B = 1;
	else
		B = U / (2 * A) - 1i*V / (2 * A);

	t_complex AxOut = (t_complex)A* sqrt(sParameters[0] *p);
	t_complex AyOut = (t_complex)B* sqrt(sParameters[0] *p);

	t_complex_xy photonOut = { AxOut,AyOut };
	return photonOut;
}





/* Recursive function for finding determinant of matrix.
   n is current dimension of A[][]. */
int ElectronicPolarizationController::determinant(const double A[N][N])
{
	double a = A[1][1] * A[2][2] - A[1][2] * A[2][1];
	double b = A[1][0] * A[2][2] - A[2][0] * A[1][2];
	double c = A[1][0] * A[2][1] - A[1][1] * A[2][0];
	int det = (int) round(A[0][0] *a - A[0][1] * b + A[0][2] * c);

	return det;
}



double ElectronicPolarizationController::determinantOfMinor(int theRowHeightY,int theColumnWidthX, const double theMatrix[N][N])
{
	int x1 = theColumnWidthX == 0 ? 1 : 0;  /* always either 0 or 1 */
	int x2 = theColumnWidthX == 2 ? 1 : 2;  /* always either 1 or 2 */
	int y1 = theRowHeightY == 0 ? 1 : 0;  /* always either 0 or 1 */
	int y2 = theRowHeightY == 2 ? 1 : 2;  /* always either 1 or 2 */

	return (theMatrix[y1][x1] * theMatrix[y2][x2])
		- (theMatrix[y1][x2] * theMatrix[y2][x1]);
}

// Function to calculate and store inverse, returns false if 
// matrix is singular 
bool ElectronicPolarizationController::inverse(double A[N][N], double (&inverse)[N][N])
{
	double det = determinant(A);

	/* Arbitrary for now.  This should be something nicer... */
	if (abs(det) < 1e-2)
	{
		memset(inverse, 0, sizeof inverse);
		return false;
	}

	double oneOverDeterminant = 1.0 / det;

	for (int y = 0; y < 3; y++)
		for (int x = 0; x < 3; x++)
		{
			/* Rule is inverse = 1/det * minor of the TRANSPOSE matrix.  *
			 * Note (y,x) becomes (x,y) INTENTIONALLY here!              */
			inverse[y][x]
				= determinantOfMinor(x, y, A) * oneOverDeterminant;

			/* (y0,x1)  (y1,x0)  (y1,x2)  and (y2,x1)  all need to be negated. */
			if (1 == ((x + y) % 2))
				inverse[y][x] = -inverse[y][x];
		}

	return true;

}


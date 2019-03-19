#include "../include/sop_rotator_20180815.h"

void SOPRotator::initialize(void) {

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	/* open a .txt report*/
	myfile.open(outputSignals[0]->getFolderName() + fileName, std::ios_base::app);
	myfile << "Theta " << " Phi  \n";
}

bool SOPRotator::runBlock(void) {

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process <= 0) return false;

	for (auto k = 0; k < process; k++) {
			
		t_complex_xy photonIn;
		inputSignals[0]->bufferGet(&photonIn);
		t_complex valueX = photonIn.x;
		t_complex valueY = photonIn.y;

		t_complex_xy photonOut{};
		t_complex valueXOut, valueYOut;

		count++;
		
		if (sopRotationType == sop_rotation_type::Stocastic) 
		{

			double sigmaSquare = 2 * PI*delta_p*(inputSignals[0]->getSymbolPeriod());
			double standardev = sigmaSquare;

			std::normal_distribution<double>distribution(0, sqrt(standardev));

			unsigned int seed = (unsigned int)chrono::system_clock::now().time_since_epoch().count();
			generator.seed(seed);

			double normAlpha{ 0.0 };

			vector< vector<t_complex> > a1sigma1 = { { 0.0,0.0 },{ 0.0,0.0 } };
			vector< vector<t_complex> > a2sigma2 = { { 0.0,0.0 },{ 0.0,0.0 } };
			vector< vector<t_complex> > a3sigma3 = { { 0.0,0.0 },{ 0.0,0.0 } };

			/*alpha*/
			alpha[0] = distribution(generator);
			alpha[1] = distribution(generator);
			alpha[2] = distribution(generator);

			/*write stokes parameters in a file*/
		/*	if (myfile.is_open()) 
			{
				myfile << std::fixed << alpha[0] << " " << alpha[1] << " " << alpha[2] << " \n";

			}*/

			/*norm(alpha)*/

			normAlpha = sqrt(pow(alpha[0], 2) + pow(alpha[1], 2) + pow(alpha[2], 2));

			/*aa - unit vector defining the rotation vector*/
			aa[0] = alpha[0] / normAlpha;
			aa[1] = alpha[1] / normAlpha;
			aa[2] = alpha[2] / normAlpha;

			/*Matrix J = I2*cos(normAlpha) - i*sin(normAlpha)*(a1*sigma1 + a2*sigma2 + a3*sigma3) */

			vector< vector<t_complex> > asigma = { { aa[0],aa[1] - (1i*aa[2]) },{ aa[1] + (1i*aa[2]),-aa[0] } };

			t_complex detsigma = asigma[0][0] * asigma[1][1] - asigma[0][1] * asigma[1][0];

			double det1 = abs(detsigma);

			for (auto line = 0; line < 2; line++)
			{
				for (auto col = 0; col < 2; col++)
				{
					JJ[line][col] = I2[line][col] * cos(normAlpha) - 1i*asigma[line][col] * sin(normAlpha);
				}
			}
			vector< vector<t_complex> > JJmult(2, vector<t_complex>(2));
			if (firstSop) {

				valueXOut = valueX * JJ[0][0] + valueY * JJ[0][1];
				valueYOut = valueX * JJ[1][0] + valueY * JJ[1][1];
				firstSop = false;
			
			}
			else 
			{
				t_complex mult = 0;
					
				/*Compute JJ*JJnext*/
				for (auto line = 0; line < 2; line++) {
					for (auto col1 = 0; col1 < 2; col1++) {
						for (auto col2 = 0; col2 < 2; col2++) {
							mult = mult + JJ[line][col2] * JJnext[col2][col1];
						}
						JJmult[line][col1] = mult;
						mult = 0;
					}
				}

				valueXOut = valueX * JJmult[0][0] + valueY * JJmult[0][1];
				valueYOut = valueX * JJmult[1][0] + valueY * JJmult[1][1];

				if (count >= timeRotation) {
					phi = 0;
					theta = acos((((1 - qber) * 2) - 1) / cos(phi));
					t_complex valueX_aux = valueXOut;
					t_complex valueY_aux = valueYOut;
					singleRotation(valueX_aux, valueY_aux, valueXOut, valueYOut);
				}

				if (myfile.is_open())
				{
					t_complex valueX_conj = conj(valueXOut);
					t_complex valueY_conj = conj(valueYOut);
					t_real S0 = real(valueXOut*valueX_conj + valueYOut * valueY_conj);
					t_real S1 = real(valueXOut*valueX_conj - valueYOut * valueY_conj);
					t_real S2 = real(valueXOut*valueY_conj + valueYOut * valueX_conj);
					t_real S3 = real(1i*(valueXOut*valueY_conj - valueX_conj * valueYOut));
					if(S0 > 0.0)
						myfile << std::fixed << S0 << " " << S1 << " " << S2 << " " << S3 << " \n";

				}
				

				for (auto line = 0; line < 2; line++) {
					for (auto col = 0; col < 2; col++) {
						JJ[line][col] = JJmult[line][col];
					}
				}
			}

			for (auto line = 0; line < 2; line++) {
				for (auto col = 0; col < 2; col++) {
					JJnext[line][col] = JJ[line][col];
				}
			}
			t_complex detJ = JJ[0][0] * JJ[1][1] - JJ[0][1] * JJ[1][0];
			double det = norm(detJ);

			if (det <= 0.9) {
				int a = 1;
			}
		}
		else 
		{
			t_complex valueX_conj = conj(valueX);
			t_complex valueY_conj = conj(valueY);
			/*Jones To Stokes*/
			t_real S0 = real(valueX*valueX_conj + valueY*valueY_conj); 
			t_real S1 = real(valueX*valueX_conj - valueY*valueY_conj); 
			t_real S2 = real(valueX*valueY_conj + valueY*valueX_conj); 
			t_real S3 = real(1i*(valueX*valueY_conj - valueX_conj *valueY));

			if (S0 == 0) {
				valueXOut = valueX;
				valueYOut = valueY;
			}
			else {
				if (firstSop) {
					if (defineQBER) {
						double theta_max = acos((((1 - qber) * 2) - 1));
						long int seed{ (long int)chrono::system_clock::now().time_since_epoch().count() };
						UniformRandomRealNumbers<> uniformeRandomNumberTheta{ seed, 0, theta_max };
						theta = uniformeRandomNumberTheta();
						srand(time(NULL));
						phi = acos((((1 - qber) * 2) - 1) / cos(theta));
						//theta = -acos((((1 - qber) * 2) - 1) / cos(0));
						//phi = 0;
					}
					firstSop = false;
					if (myfile.is_open())
					{
						myfile << std::fixed << theta << " " << phi << " \n";

					}
				}
				t_real S1_out = S1 * cos(phi)*cos(-theta) + S2 * (cos(phi)*sin(-theta) + pow(sin(phi), 2)*cos(-theta)) + S3 * (sin(phi)*sin(-theta) - sin(phi)*cos(phi)*cos(-theta));
				t_real S2_out = S1 * (-cos(phi)*sin(-theta)) + S2 * (cos(phi)*cos(-theta) - pow(sin(phi), 2)*sin(-theta)) + S3 * (sin(phi)*cos(theta) + cos(phi)*sin(phi)*sin(-theta));
				t_real S3_out = S1 * sin(phi) + S2 * (-sin(phi)*cos(phi)) + S3 * pow(cos(phi), 2);

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

				valueXOut = (t_complex)A* sqrt(S0*p);
				valueYOut = (t_complex)B* sqrt(S0*p);
			}

			
		}

		photonOut = { valueXOut, valueYOut };
		outputSignals[0]->bufferPut(photonOut);
		
	}
	
	if (myfile.is_open()) {
		myfile.close();
	}
	
	return true;
}

void SOPRotator::singleRotation(t_complex valueX, t_complex valueY, t_complex(&valuex_out), t_complex(&valuey_out)) {
	t_complex valueX_conj = conj(valueX);
	t_complex valueY_conj = conj(valueY);
	/*Jones To Stokes*/
	t_real S0 = real(valueX*valueX_conj + valueY * valueY_conj);
	t_real S1 = real(valueX*valueX_conj - valueY * valueY_conj);
	t_real S2 = real(valueX*valueY_conj + valueY * valueX_conj);
	t_real S3 = real(1i*(valueX*valueY_conj - valueX_conj * valueY));

	if (S0 == 0) {
		valuex_out = valueX;
		valuey_out = valueY;
	}
	else {
		t_real S1_out = S1 * cos(phi)*cos(-theta) + S2 * (cos(phi)*sin(-theta) + pow(sin(phi), 2)*cos(-theta)) + S3 * (sin(phi)*sin(-theta) - sin(phi)*cos(phi)*cos(-theta));
		t_real S2_out = S1 * (-cos(phi)*sin(-theta)) + S2 * (cos(phi)*cos(-theta) - pow(sin(phi), 2)*sin(-theta)) + S3 * (sin(phi)*cos(theta) + cos(phi)*sin(phi)*sin(-theta));
		t_real S3_out = S1 * sin(phi) + S2 * (-sin(phi)*cos(phi)) + S3 * pow(cos(phi), 2);

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

		valuex_out = (t_complex)A* sqrt(S0*p);
		valuey_out = (t_complex)B* sqrt(S0*p);
	}
	
}
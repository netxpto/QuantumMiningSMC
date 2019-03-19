#include "SOP_modulator_20180412.h"

void SOPModulator::initialize(void) {
	int numberOfInputSignals = (int)inputSignals.size();
	int numberOfOutputSignals = (int)outputSignals.size();

	if (numberOfInputSignals > 0) {
		tSymbolPeriod = inputSignals[0]->getSymbolPeriod();

		for (auto i = 0; i < numberOfOutputSignals; i++) {
			outputSignals[i]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
			outputSignals[i]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
			outputSignals[i]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
			outputSignals[i]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
		}
	}
	else {

		for (auto i = 0; i < numberOfOutputSignals; i++) {
			outputSignals[i]->setSymbolPeriod(1);
			outputSignals[i]->setSamplingPeriod(1);
			outputSignals[i]->setSamplesPerSymbol(1);
			outputSignals[i]->setFirstValueToBeSaved(1);
		}
	}

	/* open a .txt report*/
	myfile.open(fileName);
	myfile << "S1 " << "S2 " << "S3" << "\n";
	
}

bool SOPModulator::runBlock(void) {
	bool alive{ false };

	int process{ 0 };
	int space0 = outputSignals[0]->space();
	int space1 = outputSignals[1]->space();
	int space = min(space0, space1);

	if (numberOfInputSignals > 0) {
		int ready = inputSignals[0]->ready(); //clock
		process = min(ready, space);
	}
	else
		process = space;

	if (process <= 0) return alive;

	else {
		
		if (sopType == Stocastic) {
			/* open a .txt report*/
			myfile.open(fileName, ofstream::app);
		}
		

		for (auto k = 0; k < process; k++) {

			double mult = 0;
			double sigmaSquare = 2 * PI*delta_p*tSymbolPeriod;
			std::normal_distribution<double>distribution(mean, sqrt(sigmaSquare));
			unsigned int seed = (unsigned int)chrono::system_clock::now().time_since_epoch().count();
			generator.seed(seed);

			double normAlpha{ 0.0 };

			vector< vector<double> > kk2(3, vector<double>(3));
			vector< vector<double> > kk(3, vector<double>(3));
			vector< vector<double> > MM(3, vector<double>(3));
			vector<double> S0 = { 0.0 , 0.0 , 0.0 };

			double I3[3][3] = { { 1, 0, 0 },{ 0, 1, 0 },{ 0, 0, 1 } };


			double s1, s2, s3;
			
			switch (sopType)
			{
			case Deterministic:
				outputSignals[0]->bufferPut((t_real)theta);
				outputSignals[1]->bufferPut((t_real)phi);

				alive = true;
				break;

			case Stocastic:
				/* alpha */
				alpha[0] = distribution(generator);
				alpha[1] = distribution(generator);
				alpha[2] = distribution(generator);

				/*norm(alpha)*/
				normAlpha = sqrt(pow(alpha[0],2) + pow(alpha[1],2) + pow(alpha[2],2));

				/*aa - unit vector defining the rotation vector*/
				aa[0] = alpha[0] / normAlpha;
				aa[1] = alpha[1] / normAlpha;
				aa[2] = alpha[2] / normAlpha;
				
				/*Matrix K(a)*/
				kk[0][0] = 0.0;
				kk[0][1] = -aa[2];
				kk[0][2] = aa[1];
				kk[1][0] = aa[2];
				kk[1][1] = 0.0;
				kk[1][2] = -aa[0];
				kk[2][0] = -aa[1];
				kk[2][1] = aa[0];
				kk[2][2] = 0.0;
			

				/*First SOP for 0º*/
				if (firstSop) {
					S0[0] = 1 ;
					S0[1] = 0;
					S0[2] = 0;
					SS = S0;
					firstSop = false;
				}
				/*Calculation for Matrix M*/				
				for (auto line = 0; line < 3; line++) {
					for (auto col = 0; col < 3; col++) {
						kk2[line][col] = 0.0;
						MM[line][col] = 0.0;
					}
				}
				
				/*Compute K^2*/
				for (auto line = 0; line < 3; line++) {
						for (auto col1 = 0; col1 < 3; col1++) {
							for (auto col2 = 0; col2 < 3; col2++) {
								 mult = mult + kk[line][col2] * kk[col2][col1];
							}
							kk2[line][col1] = mult;
							mult = 0;
						}
				}

				/*Matrix M = I3 + k(a)*sin(2*norm_alpha) + K(a)^2 * (1-cos(2*norm_alpha))*/
				
				for (auto line = 0; line < 3; line++) {
					for (auto col = 0; col < 3; col++) {
						kk[line][col] = kk[line][col] * sin(2 * normAlpha);
						kk2[line][col] = kk2[line][col] * (1 - cos(2 * normAlpha));
					}
				}

				for (auto line = 0; line < 3; line++) {
					for (auto col = 0; col < 3; col++) {
						MM[line][col] = I3[line][col] + kk[line][col] + kk2[line][col];
					}
				}

				/*Theta and phi to output*/
				theta = (atan2(SS[0], sqrt(SS[1]*SS[1] + SS[2]*SS[2])) * (180 / PI))*2 ;
				//phi = atan2(SS[0], sqrt(SS[1] * SS[1] + SS[2] * SS[2])) * 180 / PI ;

				/*write stokes parameters in a file*/
				if (myfile.is_open()) {
					s1 = SS[0];
					s2 = SS[1];
					s3 = SS[2];
					myfile << std::fixed << s1 << " " << s2 << " " << s3 << " " << alpha[0] << " " << alpha[1] << " " << alpha[2] << " " << theta << " " << phi  <<" \n";
					
				}

				/*Compute SS for next sample*/
			
				for (auto line = 0; line < 3; line++) {
					SSnext[line] = 0.0;
					for (auto col = 0; col < 3; col++) {
						SSnext[line] += MM[line][col] * SS[col];
					}
				}

				SS = SSnext;

				outputSignals[0]->bufferPut((t_real)theta);
				outputSignals[1]->bufferPut((t_real)phi);

				break;

			default:
				break;
			}
		}
		if (sopType = Stocastic) {
			myfile.close();
		}
	}
	
	return alive;
}

#include "../include/random_sop_compensation_20181204.h"

void RandomSopCompensation::initialize(void) {
	for (auto k : outputSignals) {
		k->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
		k->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
		k->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	}

	if (window > 0) {
		for (auto i = 0; i < window; i++)
			windowBuffer.push_back(0);
	}
}

bool RandomSopCompensation::runBlock(void) {
	if (returnFinish)
		return false;
	int ready = inputSignals[0]->ready();

	int space{ MAX_BUFFER_LENGTH };
	for (auto k : outputSignals) {
		space = min(k->space(), space);
	}

	int process = min(ready, space);

	t_real reset{ 0.0 };
	
	if (firstTime) {
		process = min(space, transientLength);
		for (auto k = 0; k < process; k++) {
			outputSignals[0]->bufferPut(theta1);
			outputSignals[1]->bufferPut(theta2);
			outputSignals[2]->bufferPut(theta3);
		}
		/* Computing z */ // This code converges in below 10 steps, exactness chosen in order to achieve this rapid convergence
		double x1 = -2;
		double x2 = 2;
		double x3 = x2 - (erf(x2 / sqrt(2)) + 1 - alpha)*(x2 - x1) / (erf(x2 / sqrt(2)) - erf(x1 / sqrt(2)));
		double exacteness = 1e-15;

		while (abs(erf(x3 / sqrt(2)) + 1 - alpha) > exacteness)
		{
			x3 = x2 - (erf(x2 / sqrt(2)) + 1 - alpha)*(x2 - x1) / (erf(x2 / sqrt(2)) - erf(x1 / sqrt(2)));
			x1 = x2;
			x2 = x3;
		}

		z = -x3;

		firstTime = false;
		return false;
	}

	if (process <= 0) {
		/* Calculating bounds */
		double UpperBound = QBER + 1 / sqrt(receivedBits) * z  * sqrt(QBER*(1 - QBER)) + 1 / (3 * receivedBits)*(2 * z * z * (1 / 2 - QBER) + (2 - QBER));
		double LowerBound = QBER - 1 / sqrt(receivedBits) * z  * sqrt(QBER*(1 - QBER)) + 1 / (3 * receivedBits)*(2 * z * z * (1 / 2 - QBER) - (1 + QBER));

		/* Outputting a .txt report*/
		ofstream myfile;
		myfile.open(outputSignals[0]->getFolderName() + "/QBER.txt");
		myfile << "QBER = " << QBER * 100 << " %\n";
		myfile << "Upper and lower confidence bounds for " << (1 - alpha) * 100 << "% confidence level \n";
		myfile << "Upper Bound = " << UpperBound * 100 << " %\n";
		myfile << "Lower Bound = " << LowerBound * 100 << " %\n";
		myfile << "Number of errors = " << receivedBits - coincidences << "\n";
		myfile << "Number of received bits = " << receivedBits << "\n";
		myfile.close();
		return false;
	} 

	t_real signalValue;
	
	for (auto k = 0; k < process; k++) {
		if ((errors > 0) && (finalQubits <= 100) && (run == true)) {
			reset = 0.0;
			inputSignals[0]->bufferGet(&signalValue);
			qberValue = quantumBitErrorRate(signalValue);
			if (errors > 0 && finalQubits == 101)
				fBits = true;
		}
		else {
			if (receivedBits < numberOfBits) {
				inputSignals[0]->bufferGet(&signalValue);
				qberValue = quantumBitErrorRate(signalValue);
				QBER = qberValue;
			/*	if (receivedBits == numberOfBits)
					reset = 1.0;
	*/		}
			else {

				if (qberValue < qberMax) {
					opMode = operationModes::softMode;
					numberOfBits = nb;
				}
				else 
					opMode = operationModes::hardMode;

				if (opMode == operationModes::softMode) {
					int n_aux = (int)receivedBits;
					if ((n_aux % numberOfBits) == 0) {
						QBER = qberValue;
						reset = 1.0;
					}
					inputSignals[0]->bufferGet(&signalValue);
					qberValue = quantumBitErrorRate(signalValue);
				}

				double rotFactor{ 0.0 };
				switch (opMode)
				{
				case operationModes::softMode:
					if (qberValue > qberMin) {
						actuate = true;
						rotFactor = 0.1;
						modeControl = false;
					}
					else
						modeControl = true;
					break;
				case operationModes::hardMode:
					if (window < 0) {
						actuate = true;
						rotFactor = 0.5;
						modeControl = false;
					}
					else {
						window = -1;
						receivedBits = 0;
						numberOfBits = 240;
						coincidences = 0;
					}
					break;
				default:
					break;
				}

				if (actuate == true) {
					double theta_aux{ 0.0 };
					//double gamma{ 0.0 };
					switch (systemState)
					{
						// Obtain the circle of a sphere for a certain QBER
					case 0:
						circleOfaSphere(qberValue, circleQber);
						systemState = 1;
						emptyBuffer = true;
						reset = 0.0;
						if (inputSignals[0]->ready() > 0)
							inputSignals[0]->bufferGet(&signalValue);
						break;

						//Perform a rotation on the EPC and rotate the previous circle using the same angles
					case 1:
						reset = 1.0;
						theta_aux = max(theta, phi)*rotFactor;
						theta1 = theta_aux * 0.5;
						theta2 = theta_aux * 0.25;
						theta3 = theta_aux * 0.5;
						performRotation(circleQber, theta1, theta2, theta3, circleQberRotated);
						systemState = 2;
						numberOfBits = nb;
						printFirstRotation = true;
						if (inputSignals[0]->ready() > 0)
							inputSignals[0]->bufferGet(&signalValue);
						break;
						// Obtain circle of a sphere from the QBER acquired after rotation
					case 2:
						circleOfaSphere(qberValue, circleAfterRot);
						systemState = 3;
						if (inputSignals[0]->ready() > 0)
							inputSignals[0]->bufferGet(&signalValue);
						reset = 0.0;
						break;
						//Find intersection Points
					case 3:
						computeIntersectionPoints(circleQberRotated, circleAfterRot, intersectionPoints);
						printIntersectionPoints = true;
						if (intersectionPoints.size() == 0) {
							reset = 1.0;
							systemState = 0;
							actuate = false;
						}
						else {
							systemState = 4;
							if (inputSignals[0]->ready() > 0)
								inputSignals[0]->bufferGet(&signalValue);
							reset = 0.0;
						}
						break;
						// Choose randomly one of the two intersection points
					case 4:
						s = intersectionPoints.size();
						if (s > 0) {
							if (firstPoint) {
								double number = uniformeRandomNumberBetweenZeroAndOne();
								RandIndex = (number <= 0.5) ? 0 : 1;
								pointToRotateFirst = intersectionPoints[RandIndex];
								firstPoint = false;
								getAnglesToRotate(pointToRotateFirst, theta1, theta2, theta3);
								printAnglesEPC = true;
								numberOfBits = 0;
								if (opMode == operationModes::softMode)
									errors = esoft;
								else
									errors = e1;
								run = true;
								systemState = 5;
								reset = 1.0;
								emptyBuffer = true;

							}
							else if (secondPoint && !firstPoint) {
								secondIndex = (RandIndex == 0) ? 1 : 0;
								pointToSecondRotate = intersectionPoints[secondIndex];
								getAnglesToRotate(pointToSecondRotate, theta1, theta2, theta3);
								printAnglesEPC = true;
								numberOfBits = 0;
								fBits = true;
								run = true;
								secondPoint = false;
								intersectionPoints.clear();
								pointToRotateFirst = {}, pointToSecondRotate = {};
								systemState = 5;
								emptyBuffer = true;
							}
							else {
								systemState = 0;
								firstPoint = true;
								secondPoint = true;
							}
						}
						else {
							errors = e1;
							systemState = 0;
							firstPoint = true;
							secondPoint = true;
							cout << "ERROR: random_sop_compensation NO INTERSECTION POINTS! \n";
						}
						if (inputSignals[0]->ready() > 0)
							inputSignals[0]->bufferGet(&signalValue);

						break;
						// Test QBER
					case 5:
						reset = 1.0;
						systemState = (qberValue > qberMax) ? 4 : 0;
						if (systemState == 0) {
							firstPoint = true;
							secondPoint = true;
							fBits = true;
							run = true;
							actuate = false;
							if (window < 0)
								window = lengthWindow;
						}
						else {
							if (secondPoint == false) {
								printAnglesEPC = true;
								returnFinish = true;
							}
							else {
								theta1 = -5;
								theta2 = -5;
								theta3 = -5;
							}
						}
						if (inputSignals[0]->ready() > 0)
							inputSignals[0]->bufferGet(&signalValue);
						break;

					default:
						break;
					}
				}
			}
		}
		
		
		}

		if (reset == 1.0) {
			n++;

			ostringstream oss;
			time_t t = std::time(0);
			time(&t);

			std::stringstream ss;
			ss << t;
			std::string ts = ss.str();

			//oss << outputSignals[0]->getFolderName() + '/'+ ts + "_midreport" << n << ".txt";
			oss << outputSignals[0]->getFolderName() + '/' +"midreport" << n << ".txt";
			string filename = oss.str();

			/* Calculating bounds */
			//QBER = (receivedBits - coincidences) / receivedBits;
			double UpperBound = QBER + 1 / sqrt(receivedBits) * z  * sqrt(QBER*(1 - QBER)) + 1 / (3 * receivedBits)*(2 * z * z * (1 / 2 - QBER) + (2 - QBER));
			double LowerBound = QBER - 1 / sqrt(receivedBits) * z  * sqrt(QBER*(1 - QBER)) + 1 / (3 * receivedBits)*(2 * z * z * (1 / 2 - QBER) - (1 + QBER));

			/* Outputting a .txt report*/
			ofstream myfile;
			myfile.open(filename);
			myfile << "QBER = " << QBER * 100 << " %\n";
			myfile << "Upper and lower confidence bounds for " << (1 - alpha) * 100 << "% confidence level \n";
			myfile << "Upper Bound = " << UpperBound * 100 << " %\n";
			myfile << "Lower Bound = " << LowerBound * 100 << " %\n";
			//myfile << "Number of errors = " << receivedBits - coincidences << "\n";
			myfile << "Number of received bits = " << receivedBits << "\n\n";
			/*
			myfile << "Choosen Point: " << RandIndex << "\n";

			if (printFirstRotation==true) {
				myfile << "First Rotation with Theta1 = " << theta1 << ", theta2 = " << theta2 << ", theta3 = "<< theta3 <<"\n";
				printFirstRotation = false;
			}
			if (printIntersectionPoints == true) {
				myfile << "Choosen Point: " << RandIndex << "\n";
				myfile << "Point 1: s1 = " << intersectionPoints[0].s1 << ", s2 = " << intersectionPoints[0].s2 << ", s3 = " << intersectionPoints[0].s3 << ", theta = " << intersectionPoints[0].theta << ", phi = " << intersectionPoints[0].phi << "\n";
				myfile << "Point 2: s1 = " << intersectionPoints[1].s1 << ", s2 = " << intersectionPoints[1].s2 << ", s3 = " << intersectionPoints[1].s3 << ", theta = " << intersectionPoints[1].theta << ", phi = " << intersectionPoints[1].phi << "\n";
				printIntersectionPoints = false;
			}
			if (printAnglesEPC) {
				myfile << "EPC: Theta1 = " << theta1 << ", theta2 = " << theta2 << ", theta3 = " << theta3 << "\n";
				printAnglesEPC = false;
			}*/
			myfile.close();
			if (modeControl) {
				reset = 0.0;
			}
			else {
				receivedBits = 0;
				coincidences = 0;
				QBER = 0.0;
				reset = 0.0;
				finalQubits = 0;
			}
		}

		outputSignals[0]->bufferPut(theta1);
		outputSignals[1]->bufferPut(theta2);
		outputSignals[2]->bufferPut(theta3);

	return true;
}


void RandomSopCompensation::circleOfaSphere(double qber, circleFit3D(&circle)) {
	double aux_phi = 0.0;
	double aux_theta = acos((((1 - qber) * 2) - 1) / cos(aux_phi));
	point3D p1 = { cos(aux_theta)*cos(aux_phi),
					sin(aux_theta)*cos(aux_phi),
					sin(aux_phi),
					aux_theta,
					aux_phi };
	aux_theta = -aux_theta;
	point3D p2 = { cos(aux_theta)*cos(aux_phi),
					sin(aux_theta)*cos(aux_phi),
					sin(aux_phi),
					aux_theta,
					aux_phi };
	aux_theta = 0.0;
	aux_phi = acos((((1 - qber) * 2) - 1) / cos(aux_theta)); 
	point3D p3 = { cos(aux_theta)*cos(aux_phi),
					sin(aux_theta)*cos(aux_phi),
					sin(aux_phi),
					aux_theta,
					aux_phi };

	circleFit3D_(p1, p2, p3, circle);
	theta = p1.theta;
	phi = p3.phi;

}

void RandomSopCompensation::circleFit3D_(point3D p1, point3D p2, point3D p3, circleFit3D(&circle)){
	vector_ v1{};
	vector_ v2{};
	double rad{ 0.0 };
	vector_ center{ 0.0 };

	// v1, v2 describe the vectors from p1 to p2 and p3, resp.
	v1 = { p2.s1 - p1.s1, p2.s2 - p1.s2, p2.s3 - p1.s3 };
	v2 = { p3.s1 - p1.s1, p3.s2 - p1.s2, p3.s3 - p1.s3 };

	//l1, l2 describe the lengths of those vectors
	double l1 = vectorLength(v1);
	double l2 = vectorLength(v2);

	// v1n, v2n describe the normalized vectors v1 and v2
	vector_ v1n = normalizedVector(v1, l1);
	vector_ v2n = normalizedVector(v2, l2);

	// nv describes the normal vector on the plane of the circle
	vector_ nv{};
	CrossProduct(v1n, v2n,nv);

	//v2nb: orthogonalization of v2n against v1n
	vector_ v2nb = orthogonalization(v1n, v2n);

	//normalize v2nb
	double lv2nb = vectorLength(v2nb);
	v2nb = normalizedVector(v2nb, lv2nb);

	/* remark: the circle plane will now be discretized as follows
origin: p1                    normal vector on plane: nv
first coordinate vector: v1n  second coordinate vector: v2nb

calculate 2d coordinates of points in each plane
p1_2d = zeros(n,2); set per construction
p2_2d = zeros(n,2);p2_2d(:,1) = l1;  set per construction */

	double p3_2d[2]; //has to be calculated
	p3_2d[0] = v2.s1 * v1n.s1 + v2.s2 * v1n.s2 + v2.s3 * v1n.s3;
	p3_2d[1] = v2.s1 * v2nb.s1 + v2.s2 * v2nb.s2 + v2.s3 * v2nb.s3;


	/*calculate the fitting circle 
 due to the special construction of the 2d system this boils down to solving
 q1 = [0,0], q2 = [a,0], q3 = [b,c] (points on 2d circle)
 crossing perpendicular bisectors, s and t running indices:
 solve [a/2,s] = [b/2 + c*t, c/2 - b*t]
 solution t = (a-b)/(2*c)*/
	double a = l1;
	double b = p3_2d[0];
	double c = p3_2d[1];
	double t = 0.5*(a - b) / c;
	double scale1 = b / 2 + c*t; 
	double scale2 = c / 2 - b*t;

	/*center*/
	center.s1 = p1.s1 + scale1 * v1n.s1 + scale2 * v2nb.s1;
	center.s2 = p1.s2 + scale1 * v1n.s2 + scale2 * v2nb.s2;
	center.s3 = p1.s3 + scale1 * v1n.s3 + scale2 * v2nb.s3;

	/*Radius*/
	rad = sqrt(pow((center.s1-p1.s1),2)+ pow((center.s2 - p1.s2), 2)+ pow((center.s3- p1.s3), 2));

	circle = { center, rad, v1n, v2nb, p1, p2, p3 };
}

void RandomSopCompensation::CrossProduct(vector_ v1, vector_ v2, vector_(&crossP)) {

	crossP.s1 = (v1.s2*v2.s3) - (v1.s3*v2.s2);
	crossP.s2 = (v1.s3*v2.s1) - (v1.s1*v2.s3);
	crossP.s3 = (v1.s1*v2.s2) - (v1.s2*v2.s1);
}

double RandomSopCompensation::vectorLength(vector_ v) {
	double len = sqrt(v.s1*v.s1 + v.s2*v.s2 + v.s3*v.s3);
	return len;
}

RandomSopCompensation::vector_ RandomSopCompensation::normalizedVector(vector_ v, double length) {
	vector_ vresult = { v.s1 / length, v.s2 / length, v.s3 / length };
	return vresult;
}

RandomSopCompensation::vector_ RandomSopCompensation::orthogonalization(vector_ v1, vector_ v2) {
	double aux = v2.s1*v1.s1 + v2.s2*v1.s2 + v2.s3*v1.s3;
	vector_ vorthogonal = v2;

	vorthogonal = { v2.s1 - aux * v1.s1, v2.s2 - aux * v1.s2, v2.s3 - aux * v1.s3 };

	return vorthogonal;
}

void RandomSopCompensation::actualizeR(double(&Matrix)[3][3], double theta) {
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

void RandomSopCompensation::matrixEPCPlate(double M[3][3], double Mlambda[3][3], double angle) {
	double R1[3][3] = { {0,0,0} ,{0,0,0} ,{0,0,0} }; 
	double R[3][3] = { {0,0,0} ,{0,0,0} ,{0,0,0} };;
	actualizeR(R1, -angle);
	actualizeR(R, angle);

	double mult1[3][3] = { {0,0,0} ,{0,0,0} ,{0,0,0} };;
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

void RandomSopCompensation::actualizeMatrixF(double(&F)[3][3], double t1, double t2, double t3) {
	double firstPlate[3][3] = { {0,0,0} ,{0,0,0} ,{0,0,0} };
	double secondPlate[3][3] = { {0,0,0} ,{0,0,0} ,{0,0,0} };
	double thirdPlate[3][3] = { {0,0,0} ,{0,0,0} ,{0,0,0} };
	double mult_aux[3][3] = { {0,0,0} ,{0,0,0} ,{0,0,0} };

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

void RandomSopCompensation::performRotation(circleFit3D circ, double theta1, double theta2, double theta3, circleFit3D(&circresult)) {

	double rotation[3][3] = { {0,0,0} ,{0,0,0} ,{0,0,0} };
	actualizeMatrixF(rotation, theta1, theta2, theta3);

	double si1[3] = { {circ.p1.s1},{circ.p1.s2},{circ.p1.s3} };
	double si2[3] = { {circ.p2.s1},{circ.p2.s2},{circ.p2.s3} };
	double si3[3] = { {circ.p3.s1},{circ.p3.s2},{circ.p3.s3} };
	double so1[3] = { 0,0,0 };
	double so2[3] = { 0,0,0 };
	double so3[3] = { 0,0,0 };
	for (auto i = 0; i < 3; i++)
		for (auto j = 0; j < 3; j++) {
			so1[i] += rotation[i][j] * si1[j];
			so2[i] += rotation[i][j] * si2[j];
			so3[i] += rotation[i][j] * si3[j];
		}

	point3D p1_aux, p2_aux, p3_aux;
	p1_aux.s1 = so1[0];
	p1_aux.s2 = so1[1];
	p1_aux.s3 = so1[2];
	p1_aux.phi = asin(p1_aux.s3);
	//p1_aux.theta = acos((p1_aux.s1) / cos(p1_aux.phi));
	p1_aux.theta = atan2(p1_aux.s2, p1_aux.s1);

	p2_aux.s1 = so2[0];
	p2_aux.s2 = so2[1];
	p2_aux.s3 = so2[2];
	p2_aux.phi = asin(p2_aux.s3);
	//p2_aux.theta = acos((p2_aux.s1) / cos(p2_aux.phi));
	p2_aux.theta = atan2(p2_aux.s2, p2_aux.s1);

	p3_aux.s1 = so3[0];
	p3_aux.s2 = so3[1];
	p3_aux.s3 = so3[2];
	p3_aux.phi = asin(p3_aux.s3);
	//p3_aux.theta = acos((p3_aux.s1) / cos(p3_aux.phi));
	p3_aux.theta = atan2(p3_aux.s2, p3_aux.s1);

	circleFit3D_(p1_aux, p2_aux, p3_aux, circresult);

}

void RandomSopCompensation::computeIntersectionPoints(RandomSopCompensation::circleFit3D circ1, RandomSopCompensation::circleFit3D circ2, vector<point3D> (&intersectionPoints)) {
	// normal vector plane 1
	vector_ n1{};
	CrossProduct(circ1.v1, circ1.v2, n1);
	// normal vector plane 2
	vector_ n2;
	CrossProduct(circ2.v1, circ2.v2, n2);
	
	// Planes intersection
	line intersectionLine = planeIntersection(n1, circ1.center, n2, circ2.center);

	//Intersection line and sphere center (0,0,0) and radius 1
	intersectionLineSphere(intersectionLine, intersectionPoints);
	
}

RandomSopCompensation::line RandomSopCompensation::planeIntersection(vector_ n1, vector_ center1, vector_ n2, vector_ center2) {
	line line_aux{0,0};
	//CrossProduct(n1, n2, line_aux.directionVector);
	double N[3]{};
	N[0] = n1.s2 * n2.s3 - n1.s3 * n2.s2;
	N[1] = n1.s3 * n2.s1 - n1.s1 * n2.s3;
	N[2] = n1.s1 * n2.s2 - n1.s2 * n2.s1;

	line_aux.directionVector.s1 = N[0];
	line_aux.directionVector.s2 = N[1];
	line_aux.directionVector.s3 = N[2];

	// Plane 1 and Plane 2 intersect in a line
	//first determine max abs coordinate of cross product
	int max_coordinate = ((abs(N[0]) > abs(N[1])) && (abs(N[0]) > abs(N[2]))) ? 1 : 0;
	int max_coordinate1 = ((abs(N[1]) > abs(N[0])) && (abs(N[1]) > abs(N[2]))) ? 2 : 0;
	int max_coordinate2 = ((abs(N[2]) > abs(N[0])) && (abs(N[2]) > abs(N[1]))) ? 3 : 0;
	max_coordinate = max(max(max_coordinate, max_coordinate1), max_coordinate2);

	// Next, to get a point on the intersection line and
	// zero the max coord, and solve for the other two

	//double	d1 = -dotProduct(n1, center1);   //the constants in the Plane 1 equations
	//double	d2 = -dotProduct(n2, center2);   //the constants in the Plane 2 equations
	double c = 0.0;
	double N1[3] = { n1.s1, n1.s2, n1.s3 };
	double A1[3] = { center1.s1, center1.s2, center1.s3 };

	for (auto k = 0; k < 3; k++) {
		c += N1[k] * A1[k];
	}

	double d1 = -c;
	c = 0.0;
	double N2[3] = { n2.s1, n2.s2, n2.s3 };
	double A2[3] = { center2.s1, center2.s2, center2.s3 };
	for (auto k = 0; k < 3; k++) {
		c += N2[k] * A2[k];
	}

	double d2 = -c;

	switch (max_coordinate)
	{
	case 1:
		line_aux.point.s1 = 0;
		line_aux.point.s2 = (d2*N1[2] - d1 * N2[2]) / N[1];
		line_aux.point.s3 = (d1*N2[1] - d2 * N1[1]) / N[1];
		break;
	case 2:
		line_aux.point.s1 = (d1*N2[2]- d2 * N1[2]) / N[1];
		line_aux.point.s2 = 0;
		line_aux.point.s3 = (d2*N1[0] - d1 * N2[0]) / N[1];
		break;
	case 3:
		line_aux.point.s1 = (d2*N1[1] - d1 * N2[1]) / N[2];
		line_aux.point.s2 = (d1*N2[0] - d2 * N1[0]) / N[2];
		line_aux.point.s3 = 0;
		break;
	default:
		cout << "ERROR: RSCompensation Plane Intersection! \n";
		break;
	}

	return line_aux;
}

double RandomSopCompensation::dotProduct(vector_ v1, vector_ v2) {
	double result = v1.s1*v2.s1 + v1.s2*v2.s2 + v1.s3*v2.s3;

	return result;
}
/*
void RandomSopCompensation::intersectionLineSphere(line l, vector<point3D>(&intersectionP)) {

	double tol{ 1e-14 };

	// difference between centers
	vector_ dc = { l.point.s1 - 0 , l.point.s2 - 0, l.point.s3 - 0};

	// equation coefficients
	double a = l.directionVector.s1*l.directionVector.s1 + l.directionVector.s2*l.directionVector.s2 + l.directionVector.s3*l.directionVector.s3;
	double b = 2 * (l.directionVector.s1*dc.s1 + l.directionVector.s2*dc.s2 + l.directionVector.s3*dc.s3);
	double c = (pow(dc.s1,2) + pow(dc.s2,2) + pow(dc.s3,2)) - 1;

	// solve equation
	double delta = pow(b, 2) - 4 * a*c;

	if (delta > tol) {
		// delta positive: find two roots of second order equation
		double u1 = (-b - sqrt(delta)) / 2 / a;
		double u2 = (-b + sqrt(delta)) / 2 / a;

		// convert into 3D coordinate
		intersectionP.push_back(point3D());
		intersectionP[0].s1 = l.point.s1 + u1 * l.directionVector.s1;
		intersectionP[0].s2 = l.point.s2 + u1 * l.directionVector.s2;
		intersectionP[0].s3 = l.point.s3 + u1 * l.directionVector.s3;
		intersectionP[0].phi = asin(intersectionP[0].s3);
		intersectionP[0].theta = atan2(intersectionP[0].s2 , intersectionP[0].s1);
		
		intersectionP.push_back(point3D());
		intersectionP[1].s1 = l.point.s1 + u2 * l.directionVector.s1;
		intersectionP[1].s2 = l.point.s2 + u2 * l.directionVector.s2;
		intersectionP[1].s3 = l.point.s3 + u2 * l.directionVector.s3;
		intersectionP[1].phi = asin(intersectionP[1].s3);
		intersectionP[1].theta = atan2(intersectionP[1].s2 , intersectionP[1].s1);
		
	}
	else if (abs(delta) < tol) {
		//delta around zero: find unique root, and convert to 3D coord.
		double  u = -(b / 2) / a;
		intersectionP.push_back(point3D());
		intersectionP[0].s1 = l.point.s1 + u * l.directionVector.s1;
		intersectionP[0].s2 = l.point.s2 + u * l.directionVector.s2;
		intersectionP[0].s3 = l.point.s3 + u * l.directionVector.s3;
		intersectionP[0].phi = asin(intersectionP[0].s3);
		intersectionP[0].theta = atan2(intersectionP[1].s2, intersectionP[1].s1);
	}
	else {
		//delta negative : no solution
	}

}
*/
double RandomSopCompensation::quantumBitErrorRate(double sValue) {
	std::vector<char> values(dataSequence.begin(), dataSequence.end());
	int valuesSize = (int)values.size();

	t_real valueToCompare = (t_real)(values[posDataSeq] - '0');
	double qber_aux{ 0.0 };
	posDataSeq++;
	posDataSeq = posDataSeq % valuesSize;

	receivedBits++;

	if (window < 0) {
		if (sValue == valueToCompare)
			coincidences++;
		else
			errors--;
		if (run)
			finalQubits++;

		qber_aux = (receivedBits - coincidences) / receivedBits;
	}
	else {
		if (sValue == valueToCompare) {
			windowBuffer[inBuffer] = 0;
		}
		else
			windowBuffer[inBuffer] = 1;

		inBuffer = (inBuffer + 1) % window;

		double sum{ 0.0 };
		for (auto i = 0; i < window; i++)
			sum = sum + windowBuffer[i];
		coincidences = window - sum;
		qber_aux = sum / window;
		if (run)
			finalQubits++;
	}

	return qber_aux;
}

void RandomSopCompensation::getAnglesToRotate(point3D p, double (&theta1), double(&theta2), double (&theta3)) {
	double si[3] = { {p.s1},{p.s2},{p.s3} };
	
	double firstPlate[3][3] = { {0,0,0} ,{0,0,0} ,{0,0,0} };
	//double theta1 = p.theta / 2;
	theta1 = atan2(p.s2,p.s1)/2;

	matrixEPCPlate(firstPlate, mLambda4, theta1);

	double sj[3]= { 0,0,0 };
	for (auto i = 0; i < 3; i++)
		for (auto j = 0; j < 3; j++) {
			sj[i] += firstPlate[i][j] * si[j];
		}

	theta2 = (atan2(sj[1], sj[0])+atan2(0,1)) / 4;

	double secondPlate[3][3] = { {0,0,0} ,{0,0,0} ,{0,0,0} };
	matrixEPCPlate(secondPlate, mLambda2, theta2);

	double sk[3]= { 0,0,0 };
	for (auto i = 0; i < 3; i++)
		for (auto j = 0; j < 3; j++) {
			sk[i] += secondPlate[i][j] * sj[j];
		}

	theta3 = atan2(sk[1] ,sk[0]) / 2;

}

void RandomSopCompensation::intersectionLineSphere(line l, vector<point3D>(&intersectionP)) {

	double tol{ 1e-14 };
	double line_data[6] = { l.point.s1, l.point.s2, l.point.s3, l.directionVector.s1, l.directionVector.s2, l.directionVector.s3 };
	double sphere[4] = { 0,0,0,1 };
	double dc[3];
	int line_size[2] = {1,2};
	double dv0[4];
	double point_data[6];
	int point_size[2], i0;
	double x[3],a, y, b, delta, u, u1, u2, lin[6];
	bool pointFound{ true };
	// difference between centers
	for (auto k = 0; k < 3; k++) {
		dc[k] = line_data[k] - sphere[k];
		x[k] = line_data[line_size[0] * (3 + k)] * line_data[line_size[0] * (3 + k)];
	}

	a = x[0];
	for (auto k = 0; k < 2; k++) {
		a += x[k + 1];
	}

	for (auto k = 0; k < 3; k++) {
		x[k] = dc[k] * line_data[3 + k];
	}

	y = x[0];
	for (auto k = 0; k < 2; k++) {
		y += x[k + 1];
	}

	b = 2.0 * y;
	for (auto k = 0; k < 3; k++) {
		dc[k] *= dc[k];
	}

	y = dc[0];
	for (auto k = 0; k < 2; k++) {
		y += dc[k + 1];
	}

	/*  solve equation */
	delta = b * b - 4.0 * a * (y - sphere[3] * sphere[3]);
	if (delta > 1.0E-14) {
		/*  delta positive: find two roots of second order equation */
		u1 = (-b - sqrt(delta)) / 2.0 / a;
		u2 = (-b + sqrt(delta)) / 2.0 / a;

		/*  convert into 3D coordinate */
		point_size[0] = 2;
		point_size[1] = 3;
		for (auto k = 0; k < 3; k++) {
			lin[k << 1] = line_data[k] + u1 * line_data[3 + k];
			lin[1 + (k << 1)] = line_data[k] + u2 * line_data[3 + k];
			for (i0 = 0; i0 < 2; i0++) {
				point_data[i0 + (k << 1)] = lin[i0 + (k << 1)];
			}
		}
	}
	else if (fabs(delta) < 1.0E-14) {
		/*  delta around zero: find unique root, and convert to 3D coord. */
		u = -b / 2.0 / a;
		point_size[0] = 1;
		point_size[1] = 3;
		for (auto k = 0; k < 3; k++) {
			point_data[k] = line_data[k] + u * line_data[3 + k];
		}
	}
	else {
		/*  delta negative: no solution */
		point_size[0] = 2;
		point_size[1] = 3;
		for (auto k = 0; k < 6; k++) {
			point_data[k] = 0.0;
		}
		pointFound = false;
	}
	
	if (pointFound == true) {
		intersectionP.push_back(point3D());
		intersectionP[0].s1 = point_data[0];
		intersectionP[0].s2 = point_data[2];
		intersectionP[0].s3 = point_data[4];
		intersectionP[0].phi = asin(intersectionP[0].s3);
		intersectionP[0].theta = atan2(intersectionP[0].s2, intersectionP[0].s1);

		intersectionP.push_back(point3D());
		intersectionP[1].s1 = point_data[1];
		intersectionP[1].s2 = point_data[3];
		intersectionP[1].s3 = point_data[5];
		intersectionP[1].phi = asin(intersectionP[1].s3);
		intersectionP[1].theta = atan2(intersectionP[1].s2, intersectionP[1].s1);
	}
}

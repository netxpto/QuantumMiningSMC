#ifndef RANDOM_SOP_COMPENSATION_H
#define RANDOM_SOP_COMPENSATION_H

#include "../include/netxpto_20180815.h"
enum class operationModes {softMode, hardMode};

class RandomSopCompensation : public Block {

public:
	RandomSopCompensation(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setConfidence(double P) { alpha = 1 - P; }
	double const getConfidence(void) { return 1 - alpha; }

	void setDataSequence(string dSequence) { dataSequence = dSequence; };
	string getDataSequence() { return dataSequence; };

	void setWindow(long int nSamples) { window = nSamples; };
	long int getWindow() { return window; };

	void setQberInitial(double qberInit) { QBER_initial = qberInit; };
	double getQberInitial() { return QBER_initial; };

private:
	long int seed{ (long int)chrono::system_clock::now().time_since_epoch().count() };
	UniformRandomRealNumbers<> uniformeRandomNumberBetweenZeroAndOne{ seed, 0, 1 };

	//###############Internal parameters############################################################################################
	ofstream myfile2;
	string fileName2{ "/DebugParameters.txt" };
	//###############Input Parameters###############################################################################################

	double qberMax{ 0.03 };
	double qberMin{ 0.02 };
	double dTheta{ 0.25 };
	double dPhi{ 0.25 };
	int numberOfErrors{ 5 };
	int transientLength{ 32 };
	int errors{ 0 };
	int numberOfBits{ 250 };
	bool run{ false };
	bool fBits{ false };
	int nb{ 250 };
	bool returnFinish{ false };
	bool modeControl{ true };

	long int window{ 4*numberOfBits };
	long int lengthWindow{ 4 * numberOfBits };
	vector <int> windowBuffer;
	int inBuffer{ 0 };
	double alpha = 0.01;
	string dataSequence{ "0" };
	int e1{ 5 };
	int esoft{ 2 };
	bool actuate{ false };

	/***********QBER state variables***********/
	double z;
	int firstPass{ 1 };
	double coincidences = 0.0;
	double receivedBits = 0.0;
	int posDataSeq{ 0 };
	int n{ 0 };
	double reset{ 0.0 };
	double QBER{ 0.0 };
	double QBER_initial{ 0.0 };
	double qberValue{ 0.0 };
	int finalQubits{ 0 };
	bool emptyBuffer{ false };

	//###############Systen state Parameters########################################################################################
	operationModes opMode{ operationModes::softMode };

	double theta{ 0.0 };
	double phi{ 0.0 };
	bool firstPoint{ true };
	bool secondPoint{ true };
	bool firstTime{ true };
	double theta1{ 0.0 }, theta2{ 0.0 }, theta3{ 0.0 };
	
	double mLambda4[3][3] = { {1,0,0},{0,0,-1},{0,1,0} };
	double mLambda2[3][3] = { {1,0,0},{0,-1,0},{0,0,-1} };

	bool printFirstRotation{ false };
	bool printIntersectionPoints{ false };
	bool printQberEstimated{ false };
	bool printAnglesEPC{ false };

	struct point3D {
		double s1;
		double s2;
		double s3;
		double theta;
		double phi;
	};

	struct vector_ {
		double s1;
		double s2;
		double s3;
	};

	struct circleFit3D {
		vector_ center;
		double rad;
		vector_ v1;
		vector_ v2;
		point3D p1;
		point3D p2; 
		point3D p3;
	};

	struct line {
		point3D point;
		vector_ directionVector;
	};

	struct rotationAngles {
		double theta;
		double phi;
	};

	circleFit3D circleQber{};
	circleFit3D circleQberRotated{};
	circleFit3D circleAfterRot{};
	int systemState{ 0 };
	vector <point3D> intersectionPoints;
	point3D pointToRotateFirst, pointToSecondRotate;
	int RandIndex, s, secondIndex;

	
	
	//##############################################################################################################################

	void circleOfaSphere(double qber, circleFit3D(&circle));
	void performRotation(circleFit3D circ, double theta1, double theta2, double theta3, circleFit3D(&circresult));
	void computeIntersectionPoints(RandomSopCompensation::circleFit3D circ1, RandomSopCompensation::circleFit3D circ2, vector<point3D>(&intersectionPoints));
	void circleFit3D_(point3D p1, point3D p2, point3D p3, circleFit3D(&circle));
	void CrossProduct(vector_ v1, vector_ v2, vector_(&crossP));
	double vectorLength(vector_ v);
	vector_ normalizedVector(vector_ v, double length);
	vector_ orthogonalization(vector_ v1, vector_ v2);
	line planeIntersection(vector_ n1, vector_ center1, vector_ n2, vector_ center2);
	double dotProduct(vector_ v1, vector_ v2);
	void intersectionLineSphere(line l, vector<point3D>(&intersectionP));

	double quantumBitErrorRate(double sValue);

	void actualizeR(double(&Matrix)[3][3], double theta);
	void actualizeMatrixF(double(&F)[3][3], double t1, double t2, double t3);
	void matrixEPCPlate(double M[3][3], double Mlambda[3][3], double angle);
	void getAnglesToRotate(point3D p, double(&theta1), double(&theta2), double(&theta3));

};

#endif // !RANDOM_SOP_COMPENSATION_H


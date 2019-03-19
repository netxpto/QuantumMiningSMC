#ifndef ELECTRONIC_POLARIZATION_CONTOLLER_H_
#define ELECTRONIC_POLARIZATION_CONTOLLER_H_

#define N 3

#include "../include/netxpto_20180815.h"

class ElectronicPolarizationController : public Block {
public:
	ElectronicPolarizationController(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};
	ElectronicPolarizationController(vector <Signal*> &inputSignals, vector <Signal*> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);
	bool runBlock(void);

private:

	/*Input Parameters*/
	double mLambda4[3][3] = { {1,0,0},{0,0,-1},{0,1,0} };
	double mLambda2[3][3] = { {1,0,0},{0,-1,0},{0,0,-1} };
	ofstream myfile;
	string fileName{ "/gamma.txt" };

	//State system variables
	double theta1{ 0.0 }, theta2{ 0.0 }, theta3{ 0.0 };
	double matrixF[3][3];
	bool firstTime{ true };
	double previousM[3][3] = { {0,0,0},{0,0,0},{0,0,0} };

	/*Internal functions*/
	void actualizeR(double(&Matrix)[3][3], double theta);
	void actualizeMatrixF(double(&F)[3][3], double t1, double t2, double t3);
	void matrixEPCPlate(double (&M)[3][3], double (&Mlambda)[3][3], double angle);
	array <double, 4> jones2stokes(t_complex_xy photon);
	t_complex_xy stokes2jones(array<double, 4>sParameters);
	int determinant(const double A[N][N]);
	bool inverse(double A[N][N], double(&inverse)[N][N]);
	double determinantOfMinor(int theRowHeightY, int theColumnWidthX, const double theMatrix[N][N]);
};

#endif

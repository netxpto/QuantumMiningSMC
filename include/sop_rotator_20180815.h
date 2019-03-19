#ifndef SOP_ROTATOR_H
#define SOP_ROTATOR_H

/*
#include "netxpto_20180418.h"
#include <math.h> 
#include <random>
#include <chrono>
*/

#include "../include/netxpto_20180815.h"

enum class sop_rotation_type { Deterministic, Stocastic };

class SOPRotator : public Block {

public:

	//#############################################################################################################################

	SOPRotator(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);


	//#############################################################################################################################

	void setSOPRotationType(sop_rotation_type sRotationType) { sopRotationType = sRotationType; };
	sop_rotation_type getSOPRotationType() { return sopRotationType; };

	void setDeltaP(long double deltaP) { delta_p = deltaP; };
	long double getDeltaP() { return delta_p; };

	void setRotationAngle(double angle) { theta = angle; };
	double getRotationAngle() { return theta; };

	void setElevationAngle(double eAngle) { phi = eAngle; };
	double getElevationAngle() { return phi; };

	void setQBERi(double qberI) { qber = qberI; }

	void setDefineQBER(bool defineQ) { defineQBER = defineQ; };
	void singleRotation(t_complex valueX, t_complex valueY, t_complex(&valuex_out), t_complex(&valuey_out));

	//#############################################################################################################################

private:

	/*Input parameters*/
	sop_rotation_type sopRotationType{ sop_rotation_type::Deterministic };

	double theta{ 0.0 };															
	double phi{ 0.0 };	
	double qber{ 0.0 };
	long int count{ 0 };

	long double delta_p{ 7e-7 };													// Polarization linewidth
	double timeRotation{ 1000000 };

	/*Internal parameters*/
	random_engine generator;
	vector<double>alpha = { 0.0, 0.0, 0.0 };
	vector<double>aa = { 0.0, 0.0, 0.0 };
	ofstream myfile;
	string fileName{ "/Angles.txt" };
	double mean{ 0.0 };

	/*State variables*/
	bool defineQBER{ false };
	bool firstSop{ true };
	vector< vector<t_complex> > JJnext = { {0,0},{0,0} };
	vector< vector<t_complex> > JJ = { { 0,0 },{ 0,0 } };
	vector< vector<t_complex> > sigma1 = { { 1.0,0.0 },{ 0.0,1.0 } };
	vector< vector<t_complex> > sigma2 = { { 0.0, 1.0 },{ 1.0, 0.0 } };
	vector< vector<t_complex> > sigma3 = { { 0.0, -1i },{ 1i, 0.0 } };
	vector< vector<double> > I2 = { { 1.0,0.0 },{ 0.0,1.0 } };
	vector< vector<double> > I3 = { { 1.0,0.0,0.0 },{ 0.0,1.0,0.0 },{0.0,0.0,1.0} };
};

#endif 

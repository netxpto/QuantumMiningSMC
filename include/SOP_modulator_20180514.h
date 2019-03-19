#ifndef SOP_MODULATOR_H
#define SOP_MODULATOR_H

#include "netxpto_20180418.h"
#include <math.h> 
#include <random>
#include <chrono>

enum SOPType { Deterministic, Stocastic };

class SOPModulator : public Block {
	/*Input parameters*/
	SOPType sopType{ Deterministic };
	double theta{ 0.0 };								// rotation angle in degrees
	double phi{ 0.0 };									//angle phi in degrees
	long double delta_p{ 60e-4 };						// Polarization linewidth 
	
	
	/*Internal parameters*/
	std::default_random_engine generator;
	vector<double>alpha = { 0.0, 0.0, 0.0 };
	vector<double>aa = { 0.0, 0.0, 0.0 };
	ofstream myfile;
	string fileName{ "/parameters.txt" };
	double tSymbolPeriod{ 0.0 };						// symbol interval
	double mean{ 0.0 };

	/*State variables*/
	bool firstSop{ true };
	vector< vector<t_complex> > JJnext = { {0,0},{0,0} };
	vector< vector<t_complex> > JJ = { { 0,0 },{ 0,0 } };
	vector< vector<t_complex> > sigma1 = { { 1.0,0.0 },{ 0.0,1.0 } };
	vector< vector<t_complex> > sigma2 = { { 0.0, 1.0 },{ 1.0, 0.0 } };
	vector< vector<t_complex> > sigma3 = { { 0.0, -1i },{ 1i, 0.0 } };
	vector< vector<double> > I2 = { { 1.0,0.0 },{ 0.0,1.0 } };
	vector< vector<double> > I3 = { { 1.0,0.0,0.0 },{ 0.0,1.0,0.0 },{0.0,0.0,1.0} };
public:

	/*Methods*/
	SOPModulator() {};
	SOPModulator(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setDeltaP(long double deltaP) { delta_p = deltaP; };
	long double getDeltaP() { return delta_p; };

	void setSOPType(SOPType sType) { sopType = sType; };
	SOPType getSOPType() { return sopType; };

	void setRotationAngle(double angle) { theta = angle; };
	double getRotationAngle() { return theta; };

	void setElevationAngle(double eAngle) { phi = eAngle; };
	double getElevationAngle() { return phi; };


};

#endif // !SOP_MODULATOR_H

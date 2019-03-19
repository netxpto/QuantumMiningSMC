#ifndef OPTICAL_FIBER_H
#define OPTICAL_FIBER_H

#include "netxpto_20180118.h"
#include <math.h> 
#include <random>
#include <chrono>

enum SOPType{Deterministic, Stocastic};

class SOPModulator : public Block {

	/*Input Parameters*/
	SOPType sopType{ Deterministic };
	double theta{ 0.0 };								// rotation angle in degrees
	double phi{ 0.0 };									//angle phi in degrees
	long double delta_p{ 60e-4 };						// Polarization linewidth 
	double mean{ 0.0 };
	

	/*Internal parameters*/
	std::default_random_engine generator;
	vector<double>alpha = { 0.0, 0.0, 0.0 };
	vector<double>aa = { 0.0, 0.0, 0.0 };
	ofstream myfile;
	double tSymbolPeriod{ 0.0 };						// symbol interval
	string fileName{ "stokes_parameters.txt" };
	
	/*State variables*/
	bool firstSop{ true };
	vector<double> SS = {0.0, 0.0, 0.0};
	vector<double> SSnext = { 0.0 , 0.0 , 0.0 };

public:
	SOPModulator() {};
	SOPModulator(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setSOPType(SOPType sType) { sopType = sType; };
	SOPType getSOPType() { return sopType; };

	void setRotationAngle(double angle) { theta = angle; };
	double getRotationAngle() { return theta; };

	void setElevationAngle(double eAngle) { phi = eAngle; };
	double getElevationAngle() { return phi; };

	void setDeltaP(long double deltaP) { delta_p = deltaP; };
	long double getDeltaP() { return delta_p; };

	void setDistributionMean(double m) { mean = m; };
	double getDistributionMean() { return mean; };



};

#endif // !OPTICAL_FIBER_H


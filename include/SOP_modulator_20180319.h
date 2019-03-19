#ifndef OPTICAL_FIBER_H
#define OPTICAL_FIBER_H

#include "netxpto_20180118.h"
#include <math.h> 

enum SOPType{Deterministic, Stocastic};

class SOPModulator : public Block {
	/*Input Parameters*/
	SOPType sopType{ Deterministic };
	double theta{ 0.0 }; // rotation angle in degrees
	double phi{ 0.0 }; //angle phi in degrees


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

};

#endif // !OPTICAL_FIBER_H


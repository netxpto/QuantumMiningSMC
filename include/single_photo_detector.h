#pragma once
# ifndef PROGRAM_INCLUDE_SINGLE_PHOTO_DETECTOR_H_
# define PROGRAM_INCLUDE_SINGLE_PHOTO_DETECTOR_H_

# include "netxpto.h"
#include <random>

// Simulates a photodiode
class SinglePhotodiode : public Block {

	bool firstTime{ true };
	int aux = 0;
	double t = 0;
	double responsivity{ 1 };


public:

	SinglePhotodiode() {};
	SinglePhotodiode(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setResponsivity(double Responsivity) { responsivity = Responsivity; }
	double const getResponsivity(void) { return responsivity; }



};

#endif // !PROGRAM_INCLUDE_PHOTODIODE_H_

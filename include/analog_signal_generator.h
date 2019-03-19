#pragma once
# ifndef PROGRAM_INCLUDE_ANALOG_SIGNAL_GENERATOR_H_
# define PROGRAM_INCLUDE_ANALOG_SIGNAL_GENERATOR_H_


# include "netxpto.h"
# include <complex>
# include <random>
# include <vector>

class AnalogSignalGenerator : public Block {

	/* Input Parameters */

	double analogSignalAmplitude{ 1.0 };
	double analogSingalFrequency{ 1 };
	double samplingPeriod{ 1.0 };
	double symbolPeriod{ 1.0 };


	/* State Variables */

public:

	/* Methods */
	AnalogSignalGenerator() {};
	AnalogSignalGenerator(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) : Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setSamplingPeriod(double sPeriod) { samplingPeriod = sPeriod; }
	double getSamplingPeriod() { return samplingPeriod; }

	void setSymbolPeriod(double syPeriod) { symbolPeriod = syPeriod; }
	double getSymbolPeriod() { return symbolPeriod; }

	void setAnalogSignalAmplitude(double rfAmpl) { analogSignalAmplitude = rfAmpl; }
	double getRfAmplitude() { return analogSignalAmplitude; }

	void setAnalogSignalFrequency(double rfFreq) { analogSingalFrequency = rfFreq; }
	double getrfFrequency() { return analogSingalFrequency; }




};

#endif 

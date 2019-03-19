#ifndef DEMUX_1_2_H
#define DEMUX_1_2_H

#include "netxpto_20180418.h"

class Demux_1_2 : public Block {

public:
	/*input parameters*/
	double symbolPeriod{ 1 };

	/*Methods*/
	Demux_1_2() {};
	Demux_1_2(vector <Signal*> &inputSignals, vector <Signal*> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);

	bool runBlock(void);

	void setSymbolPeriod(double sPeriod) { symbolPeriod = sPeriod; };
	double getSymbolPeriod() { return symbolPeriod; };

};

#endif

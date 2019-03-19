# include <complex>
# include <fstream>
# include <iostream>
# include <math.h>
# include <stdio.h>
# include <string>
# include <strstream>
# include <algorithm>
# include <functional>

# include "netplus.h"
# include "pulse_shaper.h"

using namespace std;

/*
PulseShaper::PulseShaper(vector<Signal *> &InputSig, vector<Signal *> OutputSig) {

 2016-07-05  
  numberOfInputSignals = InputSig.size();
  numberOfOutputSignals = OutputSig.size();

  inputSignals = InputSig;
  outputSignals = OutputSig;

  outputSignals[0]->symbolPeriod = inputSignals[0]->symbolPeriod;
  outputSignals[0]->samplingPeriod = inputSignals[0]->samplingPeriod;

 2016-07-05  
  if (impulseResponse.size() == 0) {

	  double samplingPeriod = outputSignals[0]->samplingPeriod;
	  double symbolPeriod = outputSignals[0]->symbolPeriod;

	  impulseResponseLength = (int)floor(impulseResponseTimeLength * symbolPeriod / samplingPeriod);

	  impulseResponse.resize(impulseResponseLength);
	  double t, sinc;
	  ofstream fileHandler("./signals/" + impulseResponseFilename, ios::out);
	  fileHandler << "// ### HEADER TERMINATOR ###\n";
	  for (int i = 0; i < impulseResponseLength; i++) {
		  t = -impulseResponseLength / 2 * samplingPeriod + i * samplingPeriod;
		  if (t != 0) {
			  sinc = sin(PI * t / symbolPeriod) / (PI * t / symbolPeriod);
		  }
		  else {
			  sinc = 1;
		  }
		  impulseResponse[i] = sinc*cos(rollOffFactor*PI*t / symbolPeriod) / (1 - (4.0 * rollOffFactor * rollOffFactor * t * t) / (symbolPeriod * symbolPeriod));
		  fileHandler << t << " " << impulseResponse[i] << "\n";
	  };
	  fileHandler.close();

	  response.resize(impulseResponseLength, 0);
  };

}*/

void PulseShaper::initialize(void) {

	outputSignals[0]->symbolPeriod = inputSignals[0]->symbolPeriod;
	outputSignals[0]->samplingPeriod = inputSignals[0]->samplingPeriod;
	outputSignals[0]->samplesPerSymbol = inputSignals[0]->samplesPerSymbol;

	firstTime = false;

	if (impulseResponse.size() == 0) {

		double samplingPeriod = outputSignals[0]->samplingPeriod;
		double symbolPeriod = outputSignals[0]->symbolPeriod;
		double samplesPerSymbol = outputSignals[0]->samplesPerSymbol;

		impulseResponseLength = (int)floor(impulseResponseTimeLength * symbolPeriod / samplingPeriod);

		impulseResponse.resize(impulseResponseLength);
		double t, sinc;
		ofstream fileHandler("./signals/" + impulseResponseFilename, ios::out);
		fileHandler << "// ### HEADER TERMINATOR ###\n";
		for (int i = 0; i < impulseResponseLength; i++) {
			t = -impulseResponseLength / 2 * samplingPeriod + i * samplingPeriod;
			if (t != 0) {
				sinc = sin(PI * t / symbolPeriod) / (PI * t / symbolPeriod);
			}
			else {
				sinc = 1;
			}
			impulseResponse[i] = sinc*cos(rollOffFactor*PI*t / symbolPeriod) / (1 - (4.0 * rollOffFactor * rollOffFactor * t * t) / (symbolPeriod * symbolPeriod));
			fileHandler << t << " " << impulseResponse[i] << "\n";
		};
		fileHandler.close();

		response.resize(impulseResponseLength, 0);

		if (!seeBeginningOfImpulseResponse) {
			int aux = (int)(samplesPerSymbol*(((double)impulseResponseTimeLength) / 2) + 1);
			outputSignals[0]->setFirstValueToBeSaved(aux);
		}
	}

}

bool PulseShaper::runBlock(void) {

	/*2016-08-03
	if (firstTime) {

		firstTime = false;

		if (impulseResponse.size() == 0) {

			double samplingPeriod = outputSignals[0]->samplingPeriod;
			double symbolPeriod = outputSignals[0]->symbolPeriod;
			double samplesPerSymbol = outputSignals[0]->samplesPerSymbol;

			impulseResponseLength = (int)floor(impulseResponseTimeLength * symbolPeriod / samplingPeriod);

			impulseResponse.resize(impulseResponseLength);
			double t, sinc;
			ofstream fileHandler("./signals/" + impulseResponseFilename, ios::out);
			fileHandler << "// ### HEADER TERMINATOR ###\n";
			for (int i = 0; i < impulseResponseLength; i++) {
				t = -impulseResponseLength / 2 * samplingPeriod + i * samplingPeriod;
				if (t != 0) {
					sinc = sin(PI * t / symbolPeriod) / (PI * t / symbolPeriod);
				}
				else {
					sinc = 1;
				}
				impulseResponse[i] = sinc*cos(rollOffFactor*PI*t / symbolPeriod) / (1 - (4.0 * rollOffFactor * rollOffFactor * t * t) / (symbolPeriod * symbolPeriod));
				fileHandler << t << " " << impulseResponse[i] << "\n";
			};
			fileHandler.close();

			response.resize(impulseResponseLength, 0);

			if (!seeBeginningOfImpulseResponse) {
				int aux = (int)(samplesPerSymbol*(((double)impulseResponseTimeLength)/2) + 1);
				outputSignals[0]->setFirstValueToBeSaved(aux);
			}
		};

	}
	*/

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process == 0) return false;

	for (int i = 0; i < process; i++) {
		t_real val;
		(inputSignals[0])->bufferGet(&val);
		if (val != 0) {
			vector<t_real> aux(impulseResponseLength, 0.0);
			transform(impulseResponse.begin(), impulseResponse.end(), aux.begin(), bind1st(multiplies<t_real>(), val));
			transform(aux.begin(), aux.end(), response.begin(), response.begin(), plus<t_real>());
		}
		outputSignals[0]->bufferPut((t_real)(response[0]));
		rotate(response.begin(), response.begin() + 1, response.end());
		response[impulseResponseLength-1] = 0.0;
	}

	return true;
}

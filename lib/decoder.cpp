# include <algorithm>
# include <complex>
# include <iostream>
# include <fstream>
# include <random>

# include "netxpto.h"
# include "decoder.h"

using namespace std;

void Decoder::initialize(void) {

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod()/log2(m));
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSymbolPeriod() / log2(m));
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

}

void Decoder::setM(int mValue) {

	m = mValue;

	iqAmplitudes.resize(m);
	switch (m) {
	case 4:

		iqAmplitudes = { { 1.0, 1.0 },{ -1.0, 1.0 },{ -1.0, -1.0 },{ 1.0, -1.0 } };

	case 16:

		iqAmplitudes = { { -3.0, -3.0 },{ -3.0, -1.0 },{ -3.0, 1.0 },{ -3.0, 3.0 },{ -1.0, -3.0 },{ -1.0, -1.0 },{ -1.0, 3.0 },{ -1.0, 1.0 },{ 3.0, -3.0 },{ 3.0, -1.0 },{ 3.0, 3.0 },{ 3.0, 1.0 },{ 1.0, -3.0 },{ 1.0, -1.0 },{ 1.0, 3.0 },{ 1.0, 1.0 } };

	};
};

void Decoder::setIqAmplitudes(vector<t_iqValues> iqAmplitudesValues) {
	m = iqAmplitudesValues.size();
	iqAmplitudes.resize(m);
	iqAmplitudes = iqAmplitudesValues;
};

bool Decoder::runBlock(void) {

	int ready0 = inputSignals[0]->ready();
	int ready1 = inputSignals[1]->ready();
	int ready = min(ready0, ready1);

	int space0 = outputSignals[0]->space();
	int proc = (int)floor(space0 / log2(m));

	int process = min(ready, proc);

	if (process == 0) return false;

	t_real in1, in2;
	double d, minimum;
	
	for (int k = 0; k < process; k++) {
		
		inputSignals[0]->bufferGet(&in1);
		inputSignals[1]->bufferGet(&in2);

		complex<double> s_in(in1, in2);
		minimum = norm(s_in - iqAmplitudes[0]);
		int aux{ 0 };
		for (int l = 1; l < m; l++) {
			d = norm(s_in - iqAmplitudes[l]);
			if (d < minimum) {
				minimum = d;
				aux = l;
			}
		}
		// Makes the bits swtich order: example 01 is writen as 10
		/*for (int n = 0; n < log2(m); n++) {

			t_binary s_out = aux % 2; //returns the remainder of the division
			outputSignals[0]->bufferPut(s_out);
			aux = aux / 2; //returns the quocient of the division
		}*/
		t_binary s_out;
		vector<int> v((int) log2(m),0);
			/*for (int n = 0; n < log2(m); n++) {

			if (n > 0) { s_out = aux % 2; }
			
			else { s_out = aux / 2; }

			outputSignals[0]->bufferPut(s_out);
			aux = aux % 2; 
		}*/

		for (int n = 0; n < log2(m); n++) { 
			
			v[n] = aux % 2;
			aux = aux / 2;
			
			 }

		int S = (int)size(v);

		for (int k = 0; k < S; k++) {
			
			s_out = v[(S-1)-k];
			outputSignals[0]->bufferPut(s_out);
			
			 }

	}

	return true;
}
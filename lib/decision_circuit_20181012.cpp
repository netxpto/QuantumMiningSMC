# include "netxpto_20180418.h"
# include "decision_circuit_20181012.h"

using namespace std;



void DecisionCircuitMQAM::initialize(void) {

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->
	                  getFirstValueToBeSaved());
}



bool DecisionCircuitMQAM::runBlock(void) {
		
	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int process = min(ready, space);
	
	if (process == 0) return false;
	
	vector<complex <double>> ConstMap;
	if (mQAM == 4) {
		ConstMap = { { 1,-1 },{ -1,-1 },{ -1,1 },{ 1,1 } };
	}
	if (mQAM == 16) {
		ConstMap = { { -3.0, -3.0 },{ -3.0, -1.0 },{ -3.0, 3.0 },{ -3.0, 1.0 },{ -1.0, -3.0 },{ -1.0, -1.0 },{ -1.0, 3.0 },{ -1.0, 1.0 },{ 3.0, -3.0 },{ 3.0, -1.0 },{ 3.0, 3.0 },{ 3.0, 1.0 },{ 1.0, -3.0 },{ 1.0, -1.0 },{ 1.0, 3.0 },{ 1.0, 1.0 } };
	}
	double inf = std::numeric_limits<double>::infinity();	
	vector<double> distMod(mQAM, 0.0);
	complex <double> result(0.0, 0.0);
	double temp(0.0);
	int minDist(0);

	t_complex valueX(0.0, 0.0);
	t_complex Signal_in(0.0, 0.0);
	for (int i = 0; i < process; i++) {
		
		inputSignals[0]->bufferGet(&valueX);
		Signal_in = valueX;

		if (Signal_in == (0.0, 0.0)) {
			result = (0.0, 0.0);
			outputSignals[0]->bufferPut(result);
		}
		else {

			for (size_t i = 0; i < mQAM; ++i) {

				complex <double> aux = Signal_in - ConstMap.at(i);
				complex <double> aux2 = (aux.real(), aux.imag());
				distMod.at(i) = abs(Signal_in - ConstMap.at(i));
			}
			temp = distMod.at(0);
			for (size_t i = 0; i < ConstMap.size(); ++i) {

				if (temp > distMod.at(i)) {
					minDist = i;
					temp = distMod.at(i);
				}
			}

			result = ConstMap.at(minDist);
			outputSignals[0]->bufferPut(result);
		}
	}

	return true;
}

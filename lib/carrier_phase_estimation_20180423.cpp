# include "carrier_phase_estimation_20180423.h"

# include <algorithm>
# include <complex>
# include <iostream>
# include <fstream>
# include <random>
#include <math.h>

//# include "netxpto_20180118.h"
using namespace std;


/*class UnwrapFunctions
{
public:
	void Unwrap(vector<double> &PhaseIn);

};

void UnwrapFunctions::Unwrap(vector<double> &PhaseIn) {


	vector<double> PhaseWrap(PhaseIn.size(), 0);

	PhaseWrap = PhaseIn;

	for (int i = 1; i != PhaseWrap.size(); ++i) {

		double difference = PhaseWrap.at(i) - PhaseWrap.at(i - 1);
		if (difference > PI) {
			for (int k = i; k != PhaseWrap.size(); ++k) {
				PhaseIn.at(k) = PhaseIn.at(k) - 2 * PI;
			}
		}
		if (difference < -PI) {
			for (int k = i; k != PhaseWrap.size(); ++k) {
				PhaseIn.at(k) = PhaseIn.at(k) + 2 * PI;
			}
		}
	}

}*/


class DecisionCircuit
{
public:
	complex <double> DecisionCircuitQPSK(complex <double> &Signal_in);
	complex <double> DecisionCircuit16QAM(complex <double> &Signal_in);
};


complex <double> DecisionCircuit::DecisionCircuitQPSK(complex <double> &Signal_in) {

	double inf = std::numeric_limits<double>::infinity();
	vector<complex <double>> ConstMap = { { 1,-1 },{ -1,-1 },{ -1,1 },{ 1,1 } };
	//vector<complex <double>> ConstMap({ { { 0.707,0.707 },{ -0.707,0.707 },{ -0.707,-0.707 },{ 0.707,-0.707 } } });
	vector<double> distMod(ConstMap.size(), 0.0);
	complex <double> result(0.0, 0.0);
	double temp(0.0);
	int minDist(0);

	if (Signal_in == (0.0, 0.0)) {
		result = (0.0, 0.0);
		return result;
	}

	for (size_t i = 0; i < ConstMap.size(); ++i) {

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
	return result;
}

complex <double> DecisionCircuit::DecisionCircuit16QAM(complex <double> &Signal_in) {

	double inf = std::numeric_limits<double>::infinity();
	vector<complex <double>> ConstMap = { { -3.0, -3.0 },{ -3.0, -1.0 },{ -3.0, 3.0 },{ -3.0, 1.0 },{ -1.0, -3.0 },{ -1.0, -1.0 },{ -1.0, 3.0 },{ -1.0, 1.0 },{ 3.0, -3.0 },{ 3.0, -1.0 },{ 3.0, 3.0 },{ 3.0, 1.0 },{ 1.0, -3.0 },{ 1.0, -1.0 },{ 1.0, 3.0 },{ 1.0, 1.0 } };
	//vector<complex <double>> ConstMap({ { { 0.707,0.707 },{ -0.707,0.707 },{ -0.707,-0.707 },{ 0.707,-0.707 } } });
	vector<double> distMod(ConstMap.size(), 0.0);
	complex <double> result(0.0, 0.0);
	double temp(0.0);
	int minDist(0);

	if (Signal_in == (0.0, 0.0)) {
		result = (0.0, 0.0);
		return result;
	}

	for (size_t i = 0; i < ConstMap.size(); ++i) {

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
	return result;
}



/*
#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif
*/

void CarrierPhaseCompensation::initialize(void) {

	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	//outputSignals[0]->setSymbolPeriod(symbolPeriod);
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	//outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}


bool CarrierPhaseCompensation::runBlock(void) {

	//UnwrapFunctions UnWrapClass;

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	int process = min(ready, space);

	if (process == 0) return false;
	
	const complex<double> I(0, 1);
	//t_complex aux(0.0, 0.0);

	if (!methodType.compare("VV")) 
	{

		if (firstTime) {
			inBuffer.resize(nTaps);
			pIn = inBuffer.size() - 1;
			firstTime = false;
		}

		for (auto k = 0; k != process; k++)
		{
			t_complex aux;
			inputSignals[0]->bufferGet(&aux);
			inBuffer[pIn] = aux;

			aux = 0;
			for (auto k : inBuffer)
			{
				aux = aux + pow(k, 4);
			}
			aux = aux / (double)nTaps;

			auto phaseEstimation = atan2(imag(aux), real(aux));

			double difference = phaseEstimation - lastPhase;
			if (difference > PI) phaseEstimation = phaseEstimation - 2 * PI;
			if (difference < -PI) phaseEstimation = phaseEstimation + 2 * PI;
			lastPhase = phaseEstimation;

			int ind = pIn - (nTaps - 1) / 2;
			ind < 0 ? ind = inBuffer.size() - ind : ind = ind;
			outputSignals[0]->bufferPut(conj(inBuffer[ind] * exp(-I * (phaseEstimation / mQAM - PI / mQAM))));

			pIn++;
			pIn = nTaps ? pIn = 0 : pIn = pIn;
		}
		return true;
	}
/*
		t_complex var1(0.0, 0.0);
		t_complex var2(0.0, 0.0);
		vector<complex <double>> bufferInput(process + nTaps - 1);
		vector<complex <double>> bufferOutput(process);
		vector <double> phaseEstimation(process);

		for (int i = 0; i < process; i++) {

			inputSignals[0]->bufferGet(&var1);
			bufferInput.at((nTaps-1)/2 + i) = pow(var2, 4);
		}

		for (int i = 0; i < process; ++i) {
			t_complex aux(0.0, 0.0);
			for (int k = 0; k < nTaps; ++k) {

				aux = aux + bufferInput.at(i + k);
			}
			aux = aux / double(nTaps);
			phaseEstimation.at(i) = atan2(imag(aux) , real(aux));
		}

		UnWrapClass.Unwrap(phaseEstimation);

		for (int i = 0; i < process; ++i) {

			inputSignals[0]->bufferGet(&var2);
			bufferOutput.at(i) = var2 * exp(-I * (phaseEstimation.at(i) / 4 - M_PI/4));
		}
		for (int i = 0; i < process; i++) {

			outputSignals[0]->bufferPut(bufferOutput.at(i));

		}
*/


	if (!methodType.compare("BPS")) {

		DecisionCircuit DC;

		t_real phiTest = 0;
		if (firstTime) 
		{
			inBuffer.resize(nTaps);
			pIn = inBuffer.size() - 1;
			firstTime = false;
		}

		for (auto k = 0; k != process; k++)
		{
			vector <complex <double>> Signal_ref(nTestPhase);
			vector <double> auxVet(nTestPhase,0.0);
			t_complex aux, Signal_rot;
			t_real phiTestSel = 0.0;

			inputSignals[0]->bufferGet(&aux);
			inBuffer[pIn] = aux;
			aux = 0;

			for (int b = 0; b < nTestPhase; b++)
			{
				phiTest = (double(b) / double(nTestPhase) - 1.0 / 2.0) * phiInt;
				for (auto k : inBuffer)
				{
					Signal_rot = k * exp(I * phiTest);
					if (mQAM == 4) {
						Signal_ref.at(b) = DC.DecisionCircuitQPSK(Signal_rot);
					}
					if (mQAM == 16) {
						Signal_ref.at(b) = DC.DecisionCircuit16QAM(Signal_rot);
					}
					auxVet.at(b) = auxVet.at(b) + pow(abs(Signal_rot - Signal_ref.at(b)), 2);
				}
				auxVet.at(b) = auxVet.at(b) / (double)nTaps;
			}
			double aux2 = 1000.0;
			int index_phaseTest = -1;

			for (int b = 0; b < nTestPhase; b++) 
			{
				if (auxVet.at(b) >= 0.0 && auxVet.at(b) < aux2) {
					aux2 = auxVet.at(b);
					index_phaseTest = b;
				}
			}
			if (!BPStype.compare("A"))
			{
				auto phaseEstimation = (double(index_phaseTest) / double(nTestPhase) - 1.0 / 2.0) * phiInt;

				double difference = phaseEstimation - lastPhase;
				if (difference > PI) phaseEstimation = phaseEstimation - 2 * PI;
				if (difference < -PI) phaseEstimation = phaseEstimation + 2 * PI;
				lastPhase = phaseEstimation;


				int ind = pIn - (nTaps - 1) / 2;
				ind < 0 ? ind = inBuffer.size() - ind : ind = ind;
				outputSignals[0]->bufferPut(conj(inBuffer[ind] * exp(I * (phaseEstimation))));
			}
			if (!BPStype.compare("B")) 
			{
				outputSignals[0]->bufferPut(conj(Signal_ref.at(index_phaseTest)));
			}
			pIn++;
			pIn = nTaps ? pIn = 0 : pIn = pIn;
		}
		return true;
		/*for (int b = 0; b < nTestPhase; b++) {

			phiTest = (double(b) / double(nTestPhase) - 1.0 / 2.0) * phiInt;

			for (int i = 0; i < process; i++) {

				inputSignals[0]->bufferGet(&var);
				Signal_rot = var * exp(I * phiTest);
				if (mQAM == 4) {
					Signal_ref[b][i] = DC.DecisionCircuitQPSK(Signal_rot);
				}
				if (mQAM == 16) {
					Signal_ref[b][i] = DC.DecisionCircuit16QAM(Signal_rot);
				}
				bufferABSdiff[b][(nTaps - 1) / 2 + i] = pow(abs(Signal_rot - Signal_ref[b][i]), 2);
			}						
		}

		for (int b = 0; b < nTestPhase; b++) {

			for (int i = 0; i < process; ++i) {

				double aux(0.0);
				for (int k = 0; k < nTaps; ++k) {

					aux = aux + bufferABSdiff[b][i + k];
				}
				aux = aux / double(nTaps);
				phaseEstimationBPS[b][i] = aux;
								
			}
		}

		for (int i = 0; i < process; ++i) {

			double aux = 1000.0;
			index_phaseTest.at(i) = -1;
			
			for (int b = 0; b < nTestPhase; b++) {
				if (phaseEstimationBPS[b][i] >= 0.0 && phaseEstimationBPS[b][i] < aux ) {
					aux = phaseEstimationBPS[b][i];
					index_phaseTest.at(i) = b;
				}
			}
			phiTestSel.at(i) = (double(index_phaseTest.at(i)) / double(nTestPhase) - 1.0 / 2.0) * phiInt;
		}
		if (!BPStype.compare("A")) {

			UnWrapClass.Unwrap(phiTestSel);

			for (int i = 0; i < process; i++) {
				inputSignals[0]->bufferGet(&var);
				bufferOutput.at(i) = var * exp(I * (phiTestSel.at(i)));
				outputSignals[0]->bufferPut(bufferOutput.at(i));
			}
			return true;
		}
		if (!BPStype.compare("B")) {

			for (int i = 0; i < process; i++) {				
				bufferOutput.at(i) = Signal_ref[index_phaseTest.at(i)][i];
				outputSignals[0]->bufferPut(bufferOutput.at(i));
			}
			return true;
		}*/
		//return true;
	}
	cout << "ERROR: Undefined CPE method \n";
	return false;
		
}

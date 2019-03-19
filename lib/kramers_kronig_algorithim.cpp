#include <algorithm>
#include <complex>
#include <chrono>
#include <fstream>

#include "netxpto.h"
#include "kramers_kronig_algorithim.h"
#include <random>
# include "overlap_save_20180208.h"
# include "fft_20180208.h"

using namespace std;

void KramersKronigAlgorithim::initialize(void) {

	firstTime = false;

	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}


bool KramersKronigAlgorithim::runBlock(void) {

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();
	int process = min(ready, space);
	if (process == 0) return false;


	switch (type)
	{
	case frequency_domain:
	{
		// put code here

		//////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////
		vector<t_complex> currentCopy(process); // Get the Input signal
		t_real input;
		vector<t_complex> currentCopyMagnitude(process);
		for (int i = 0; i < process; i++) {
			inputSignals[0]->bufferGet(&input);
			currentCopyMagnitude.at(i) = { sqrt(input), 0 };
			currentCopy.at(i) = { log(sqrt(input)),0 }; // Take the square root of "input" and its natural LOG value
		}

		if (K == 0) { previousCopy = currentCopy; }if (K == 0) { previousCopy = currentCopy; }

		///////////////////////////////////////////////// For making currentCopy equal to the previousCopy
		if (previousCopy.size() != currentCopy.size())
		{
			vector<t_complex> currentCopyAux;
			while (currentCopyAux.size() <= previousCopy.size())
			{
				for (int i = 0; i < currentCopy.size(); i++)
				{
					currentCopyAux.push_back(currentCopy[i]);
				}
			}

			vector<t_complex> cc(previousCopy.size());
			for (int i = 0; i < previousCopy.size(); i++)
			{
				cc[i] = currentCopyAux[i];
			}

			currentCopy = cc;
		}

		//////////////////////////////// Transfer Function ///////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////
		size_t tfSize = getTransferFunctionLength();
		//size_t tfSize = (previousCopy.size() + currentCopy.size())/1; // Adjust the size of the transfer manually
		vector<t_complex>  tf(tfSize);
		t_complex dcContent = { 0,0 };
		t_complex postiveFrequency = { 0,-1 };
		t_complex negativeFrequency = { 0,1 };

		for (int i = 0; i < tfSize; i++)
		{
			if (i == 0)
			{
				tf[i] = dcContent;

			}
			else if (i>0 && i<tfSize / 2)
			{
				tf[i] = postiveFrequency;

			}
			else
			{
				tf[i] = negativeFrequency;
			}
		}

		//////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////    overlap save   ///////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////

		vector<t_complex> out = overlapSaveTransferFunction(currentCopy, previousCopy, tf);

		//////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////// Exponential Table ////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		vector<t_complex> phaseValue(out.size());

		for (int i = 0; i < out.size(); i++)
			phaseValue[i] = exp(complex<double>(0, out[i].real()));

		//////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////// Multiplication /////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////

		if (L == 0)
		{
			previousCopyMagnitude = currentCopyMagnitude;
		}

		if (previousCopyMagnitude.size() != currentCopyMagnitude.size())
		{
			vector<t_complex> currentCopyMagnitudeAux;
			while (currentCopyMagnitudeAux.size() <= previousCopyMagnitude.size())
			{
				for (int i = 0; i < currentCopyMagnitude.size(); i++)
				{
					currentCopyMagnitudeAux.push_back(currentCopyMagnitude[i]);
				}
			}

			vector<t_complex> ccMagnitude(previousCopyMagnitude.size());
			for (int i = 0; i < previousCopyMagnitude.size(); i++)
			{
				ccMagnitude[i] = currentCopyMagnitudeAux[i];
			}

			currentCopyMagnitude = ccMagnitude;

		}

		vector<t_complex> magnitudeValue;
		magnitudeValue.reserve(previousCopyMagnitude.size() + currentCopyMagnitude.size());
		magnitudeValue.insert(magnitudeValue.end(), previousCopyMagnitude.begin(), previousCopyMagnitude.end());
		magnitudeValue.insert(magnitudeValue.end(), currentCopyMagnitude.begin(), currentCopyMagnitude.end());


		vector<t_complex> outF(out.size());
		for (int i = 0; i < out.size(); i++)
		{
			outF[i] = magnitudeValue[previousCopy.size()-tfSize/4 + i] * phaseValue[i];
		}

		//////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////// Bufferput ///////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		if (L == 0)
		{
			for  (int i = 0; i < process - tfSize/ 4; i++)
			{
				outputSignals[0]->bufferPut(t_complex(outF[tfSize / 4 + i])); 
			}
		}
		else
		{
			for (int i = 0; i < process; i++)
			{
				outputSignals[0]->bufferPut(t_complex(outF[ i]));
			}
		}

		/*
		//////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////// Real time Hilbert ////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////

		int span = (hilbertTransferImpulseResponseLength) / 2;		// Impulse response of the Hilbert transformer
		vector<t_complex> hn(hilbertTransferImpulseResponseLength);

		for (int i = -span; i < span; i++)
		{
		if (i % 2 == 0)
		hn.at(i + span) = { 0,0 };
		else
		hn.at(i + span) = { 2 / (i*PI),0 };
		}

		// Apply overlap and save method

		if (K == 0) { previousCopy = currentCopy; }

		vector<t_complex> dataBlock;
		dataBlock.reserve(previousCopy.size()+currentCopy.size());
		dataBlock.insert(dataBlock.end(),previousCopy.begin(),previousCopy.end());
		dataBlock.insert(dataBlock.end(), currentCopy.begin(), currentCopy.end());
		vector<t_complex> out = overlapSaveFinal(currentCopy,previousCopy, hn);

		//////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////// Exponential Table ////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		vector<t_complex> phaseValue(process);

		for (int i = 0; i < process; i++)
		phaseValue[i] = exp( complex<double>(0, out[i].real()) );

		//////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////// Multiplication /////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		vector<t_complex> previousMagnitude;

		if (L == 0)
		{
		for (int i = 0; i < span; i++)
		previousMagnitude.push_back({1,0});
		}
		else
		{
		for (int i = 0; i < span; i++)
		previousMagnitude.push_back( previousCopyMagnitude[previousCopyMagnitude.size() - span + i] );
		}

		vector<t_complex> currentMagnitude;
		for (int i = 0; i < process - span; i++)
		currentMagnitude.push_back(currentCopyMagnitude[i]);

		vector<t_complex> magnitudeValue;
		magnitudeValue.reserve(previousMagnitude.size()+currentMagnitude.size());
		magnitudeValue.insert(magnitudeValue.end(), previousMagnitude.begin(), previousMagnitude.end());
		magnitudeValue.insert(magnitudeValue.end(), currentMagnitude.begin(), currentMagnitude.end());

		vector<t_complex> outF(process);
		for (int i = 0; i < process; i++)
		{
		outF[i] = magnitudeValue[i]*phaseValue[i];
		}


		//////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////// Bufferput ///////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		if (L == 0)
		{
		for (int i = 0; i < process-span; i++){
		outputSignals[0]->bufferPut(t_complex(outF[span + i]));}
		}
		else
		{
		for (int i = 0; i < process; i++) {
		outputSignals[0]->bufferPut(t_complex(outF[i]));
		}
		}*/

		previousCopy = currentCopy;
		previousCopyMagnitude = currentCopyMagnitude;
		K = K + 1;
		L = L + 1;

		break;
	}
	case time_domain:
	{
		/*
		// put code here
		t_real input;
		vector<t_real> Iout(process);
		for (int i = 0; i < process; i++)
		{
		inputSignals[0]->bufferGet(&input);
		Iout.at(i) = input;
		}

		vector<t_real> A(process);
		A = Iout;

		vector<t_real> B(process);
		for (int i = 0; i < process; i++)
		B[i] = sqrt(A[i]);

		t_real Z = 2; // 2

		vector<t_real> C(process);
		for (int i = 0; i < process; i++)
		C[i] = A[i] / Z;

		t_real Eo = 500.7381546329677; // Eo

		vector<t_real> D(process);
		for (int i = 0; i < process; i++)
		D[i] = B[i] / Eo;


		vector<t_real> E(process);
		for (int i = 0; i < process; i++)
		E[i] = C[i] / (Eo*Eo);

		vector<t_real> F(process);
		for (int i = 0; i < process; i++)
		F[i] = D[i] * Z;

		vector<t_real> realG(process);
		for (int i = 0; i < process; i++)
		realG[i] = F[i] - E[i];
		vector<t_real> imagG(process);

		vector<t_complex> G(process);
		for (int i = 0; i < process; i++)
		G[i] = reIm2Complex(realG[i], imagG[i]);


		vector<t_complex> complexH(process);
		complexH = hilbert(G);

		vector<t_real> H(process);
		for (int i = 0; i < process; i++)
		H[i] = imag(complexH[i]);

		vector<t_real> I(process);
		for (int i = 0; i < process; i++)
		I[i] = H[i] * H[i];

		vector<t_real> J(process);
		for (int i = 0; i < process; i++)
		J[i] = I[i] / Z;

		vector<t_real> K(process);
		for (int i = 0; i < process; i++)
		K[i] = D[i] - J[i];

		vector<t_real> realL(process);
		for (int i = 0; i < process; i++)
		realL[i] = K[i] * Eo;
		vector<t_real> imagL(process);


		vector<t_complex> L(process);
		for (int i = 0; i < process; i++)
		L[i] = reIm2Complex(realL[i], imagL[i]);

		vector<t_complex> M(process);
		M = hilbert(L);

		for (auto i = 0; i < process; i++)
		outputSignals[0]->bufferPut(t_complex(M[i]));

		*/

		vector<t_complex> currentCopy(process); // Get the Input signal
		t_real input;
		vector<t_complex> currentCopyMagnitude(process);

		for (int i = 0; i < process; i++)
		{
			inputSignals[0]->bufferGet(&input);
			currentCopyMagnitude.at(i) = { sqrt(input), 0 };
			currentCopy.at(i) = { log(sqrt(input)),0 }; // Take the square root of "input" and its natural LOG value
		}

		//////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////// Real time Hilbert ////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////

		int span = (hilbertTransferImpulseResponseLength) / 2;		// Impulse response of the Hilbert transformer
		vector<t_complex> hn(hilbertTransferImpulseResponseLength);

		for (int i = -span; i < span; i++)
		{
			if (i % 2 == 0)
				hn.at(i + span) = { 0,0 };
			else
				hn.at(i + span) = { 2 / (i*PI),0 };
		}

		ofstream irHilbert("irHilbert.txt");
		irHilbert.precision(15);
		for (int i = 0; i < hn.size(); i++)
		{
			irHilbert << hn[i] << "\n";
		}
		irHilbert.close();

		// Apply overlap and save method

		if (K == 0) { previousCopy = currentCopy; }

		vector<t_complex> dataBlock;
		dataBlock.reserve(previousCopy.size() + currentCopy.size());
		dataBlock.insert(dataBlock.end(), previousCopy.begin(), previousCopy.end());
		dataBlock.insert(dataBlock.end(), currentCopy.begin(), currentCopy.end());
		vector<t_complex> out = overlapSaveImpulseResponse(currentCopy, previousCopy, hn);

		//////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////// Exponential Table ////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		vector<t_complex> phaseValue(process);

		for (int i = 0; i < process; i++)
			phaseValue[i] = exp(complex<double>(0, out[i].real()));

		//////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////// Multiplication /////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		vector<t_complex> previousMagnitude;

		if (L == 0)
		{
			for (int i = 0; i < span; i++)
				previousMagnitude.push_back({ 1,0 });
		}
		else
		{
			for (int i = 0; i < span; i++)
				previousMagnitude.push_back(previousCopyMagnitude[previousCopyMagnitude.size() - span + i]);
		}

		vector<t_complex> currentMagnitude;
		for (int i = 0; i < process - span; i++)
			currentMagnitude.push_back(currentCopyMagnitude[i]);

		vector<t_complex> magnitudeValue;
		magnitudeValue.reserve(previousMagnitude.size() + currentMagnitude.size());
		magnitudeValue.insert(magnitudeValue.end(), previousMagnitude.begin(), previousMagnitude.end());
		magnitudeValue.insert(magnitudeValue.end(), currentMagnitude.begin(), currentMagnitude.end());

		vector<t_complex> outF(process);
		for (int i = 0; i < process; i++)
		{
			outF[i] = magnitudeValue[i] * phaseValue[i];
		}


		//////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////// Bufferput ///////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		if (L == 0)
		{
			for (int i = 0; i < process - span; i++) {
				outputSignals[0]->bufferPut(t_complex(outF[span + i]));
			}
		}
		else
		{
			for (int i = 0; i < process; i++) {
				outputSignals[0]->bufferPut(t_complex(outF[i]));
			}
		}

		previousCopy = currentCopy;
		previousCopyMagnitude = currentCopyMagnitude;
		K = K + 1;
		L = L + 1;

		break;
	}

	default:
		break;
	}



	return true;
}

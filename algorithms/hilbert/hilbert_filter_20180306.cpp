
# include "hilbert_filter_20180306.h"
# include "overlap_save_20180208.h"


using namespace std;

/////////////////////////////////////////////////////////////
////////////////// Initialize HilbertFilter /////////////////
/////////////////////////////////////////////////////////////

void hilbertDesign(vector<t_complex> &transferFunction, int transferFunctionLength); // hilbertDesign declaration

void HilbertTransform::initialize(void) {
	

	double samplingPeriod = inputSignals[0]->samplingPeriod;
	double symbolPeriod = inputSignals[0]->symbolPeriod;

	outputSignals[0]->symbolPeriod = inputSignals[0]->symbolPeriod;
	outputSignals[0]->samplingPeriod = inputSignals[0]->samplingPeriod;

	transferFunctionLength = (int)floor(transferFunctionFrequencyLength * symbolPeriod / samplingPeriod);

	if (getSeeBeginningOfTransferFunction()) {
		int aux = (int)(((double)transferFunctionLength) / 2) + 1;
		outputSignals[0]->setFirstValueToBeSaved(aux);
	}
	hilbertDesign(transferFunction, transferFunctionLength);
}

void hilbertDesign(vector<t_complex> &transferFunction, int transferFunctionLength)
{
	auto N = transferFunctionLength;
	vector<t_real> transferFunctionReal(N);
	vector<t_real> transferFunctionImag;
	// Following is the MATLAB way.
	/*for (auto i=0; i<N; i++)
	{
		if (i == 0 || i == floor(N / 2))
		{
			transferFunctionReal.push_back(1);
		}
		else if (i > 0 && i <= floor(N / 2)-1)
		{
			transferFunctionReal.push_back(2);
		}
		else
		{
			transferFunctionReal.push_back(0);
		}
	}*/

	// Following is the definition based
	for (auto i = 0; i<N; i++)
	{
		if (i == 0 )
		{
			transferFunctionImag.push_back(0);
		}
		else if (i>0 && i<N/2)
		{
			transferFunctionImag.push_back(1);
		}
		else
		{
			transferFunctionImag.push_back(-1);
		}
	}

	transferFunction = reImVect2ComplexVector(transferFunctionReal,transferFunctionImag);
} //  // hilbertDesign definition


  /////////////////////////////////////////////////////////////
  /////////////////// runBlock HilbertFilter //////////////////
  /////////////////////////////////////////////////////////////

bool HilbertTransform::runBlock()
{
	bool alive{ false };
	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();
	int process = min(ready, space);
	if (process == 0) return false;

	vector<double> realInput(process); // Get the Input signal
	t_real input;
	for (int i = 0; i < process; i++) {
		inputSignals[0]->bufferGet(&input);
		realInput.at(i) = input;}

	vector<t_real> imagInput(process);
	vector<t_complex> currentCopyAux = reImVect2ComplexVector(realInput, imagInput);// currentCopyAux complex form

	vector<t_complex> currentCopy;

	if (K == 0)
	{

		for (unsigned int i = 0; i < currentCopyAux.size(); i++)
		{ currentCopy.push_back(currentCopyAux[i]); }
		previousCopy1 = currentCopy;

		K = K + 1;
		return true;
	}
	else if (K == 1)
	{
		for (unsigned int i = 0; i < currentCopyAux.size(); i++)
		{
			currentCopy.push_back(currentCopyAux[i]);
		}
		previousCopy2 = currentCopy;

		K = K + 1;
		return true;
	}

	else
	{
		for (unsigned int i = 0; i < currentCopyAux.size(); i++) { currentCopy.push_back(currentCopyAux[i]); }

		vector<complex<double>> dataBlock; // previousCopy1, previousCopy2 and currentCopy concotanation
		dataBlock.reserve(previousCopy1.size() + previousCopy2.size() + currentCopy.size());
		dataBlock.insert(dataBlock.end(), previousCopy1.begin(), previousCopy1.end());
		dataBlock.insert(dataBlock.end(), previousCopy2.begin(), previousCopy2.end());
		dataBlock.insert(dataBlock.end(), currentCopy.begin(), currentCopy.end());

		vector<t_complex> A = dataBlock;
		vector<t_complex> B = ifft(A);
		vector<t_complex> C(B.size());
		vector<t_complex> D;
		vector<t_real> E(B.size());

		t_complex Z = { 2,0 }; // COmplex Multiplier

		for (auto i = 0; i < B.size(); i++)
		{
			if (i == 0)
				C[i] = B[i];
			else if (i>0 && i<B.size() / 2)
				C[i] = Z*B[i];
			else
				C[i] = 0;
		}

		D = fft(C);

		for (auto i = 0; i < B.size(); i++)
			E[i] = imag(D[i]);


		/*vector<complex<double>> yAux;
		int newSizeOfTransferFunction = static_cast<int>(dataBlock.size());
		hilbertDesign(transferFunction, newSizeOfTransferFunction);  // Chnage transfer function size to dataBlock size.
		yAux = overlapSaveTFN(dataBlock, transferFunction); // Apply circular convolution

		// Discard calculation
		size_t frontDiscard = previousCopy1.size();
		size_t backDiscard = currentCopy.size();


		// Here we need to set a flag to acknowledge the end of the input sequence
		vector<t_complex> OUT;
		//for (int i = 0; i < previousCopy2.size(); i++) { OUT.push_back(yAux[frontDiscard + i]); } // In this case, we'll loose last frame
		// The reason to use 15 is ==> we have total 8000 symbols. Considering bufferLength = 512, we can get 
		// it 16 consecutive blocks (16*512 = 8192).
			*/
		vector<t_real> OUT;
		if (L == 0)
		{
			for (int i = 0; i < previousCopy1.size() + previousCopy2.size(); i++)
			{ OUT.push_back(E[i]); }
		}

		else 
		{
			for (int i = 0; i < previousCopy2.size(); i++)
			{ OUT.push_back(E[previousCopy1.size() + i]); }

		}
	
		// Bufferput
		for (int i = 0; i < OUT.size(); i++) {
			outputSignals[0]->bufferPut((t_real)(OUT[i]));
		}

		previousCopy1 = previousCopy2;
		previousCopy2 = currentCopy;
		K = K + 1;
		L = L + 1;
		return true;

	}

}


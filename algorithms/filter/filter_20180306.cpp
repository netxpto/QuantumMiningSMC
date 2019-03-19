# include "filter_20180306.h"

/////////////////////////////////////////////////////////////
//////////////////////// FIR_Filter ///////////////////////// TIME DOMAIN
/////////////////////////////////////////////////////////////

void FIR_Filter::initializeFIR_Filter(void) {

	outputSignals[0]->symbolPeriod = inputSignals[0]->symbolPeriod;
	outputSignals[0]->samplingPeriod = inputSignals[0]->samplingPeriod;
	outputSignals[0]->samplesPerSymbol = inputSignals[0]->samplesPerSymbol;

	if (!getSeeBeginningOfImpulseResponse()) {
		int aux = (int)(((double)impulseResponseLength) / 2) + 1;
		outputSignals[0]->setFirstValueToBeSaved(1);
	}

	delayLine.resize(impulseResponseLength, 0);

	if (saveImpulseResponse) {
		ofstream fileHandler("./signals/" + impulseResponseFilename, ios::out);
		fileHandler << "// ### HEADER TERMINATOR ###\n";

		t_real t;
		double samplingPeriod = inputSignals[0]->samplingPeriod;
		for (int i = 0; i < impulseResponseLength; i++) {
			t = -impulseResponseLength / 2 * samplingPeriod + i * samplingPeriod;
			fileHandler << t << " " << impulseResponse[i] << "\n";
		}
		fileHandler.close();
	}

};


bool FIR_Filter::runBlock(void) {
	
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
			transform(aux.begin(), aux.end(), delayLine.begin(), delayLine.begin(), plus<t_real>());
		}
		outputSignals[0]->bufferPut((t_real)(delayLine[0]));
		rotate(delayLine.begin(), delayLine.begin() + 1, delayLine.end());
		delayLine[impulseResponseLength - 1] = 0.0;
	}

	return true;
};

////// Overlap save :: Impulse Response : 2018-03-06 ////////
/////////////////////////////////////////////////////////////
///////////////////////// FD_Filter ///////////////////////// 
/////////////////////////////////////////////////////////////
void FD_Filter::initializeFD_Filter(void)
{
	outputSignals[0]->symbolPeriod = inputSignals[0]->symbolPeriod;
	outputSignals[0]->samplingPeriod = inputSignals[0]->samplingPeriod;
	outputSignals[0]->samplesPerSymbol = inputSignals[0]->samplesPerSymbol;

	if (!getSeeBeginningOfImpulseResponse()) {
		int aux = (int)(((double)impulseResponseLength) / 2) + 1;
		outputSignals[0]->setFirstValueToBeSaved(aux);
	}

	if (saveImpulseResponse) {
		ofstream fileHandler("./signals/" + impulseResponseFilename, ios::out);
		fileHandler << "// ### HEADER TERMINATOR ###\n";

		t_real t;
		double samplingPeriod = inputSignals[0]->samplingPeriod;
		for (int i = 0; i < impulseResponseLength; i++) {
			t = -impulseResponseLength / 2 * samplingPeriod + i * samplingPeriod;
			fileHandler << t << " " << impulseResponse[i] << "\n";
		}
		fileHandler.close();
	}
}

bool FD_Filter::runBlock(void)
{
	bool alive{ false };

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();
	int process = min(ready, space);
	if (process == 0) return false;
	
	/////////////////////// currentCopy //////////////////////////
	//////////////////////////////////////////////////////////////
	vector<t_complex> currentCopy(process); // Get the Input signal
	t_real input;
	for (int i = 0; i < process; i++){
		inputSignals[0]->bufferGet(&input);
		currentCopy.at(i) = { input,0 };
	}

	///////////////////////// Impulse response ////////////////////
	///////////////////////////////////////////////////////////////
	vector<t_complex> impulseResponseComplex(impulseResponse.size());
	vector<t_real> irImag(impulseResponse.size());

	impulseResponseComplex = reImVect2ComplexVector(impulseResponse, irImag);

	////////////////////// OverlapSave function ////////////////////
	////////////////////////////////////////////////////////////////
	vector<t_complex> pcinitialize(process); 
	if (K == 0) 
	{
		if (symmetryTypeIr == "Symmetric")
			previousCopy = pcinitialize;

		else
			previousCopy = currentCopy;
	}

	vector<t_complex> OUT = overlapSaveImpulseResponse(currentCopy, previousCopy, impulseResponseComplex);

	previousCopy = currentCopy;
	K = K + 1;

	// Bufferput
	for (int i = 0; i < process; i++){
		t_real val;
		val = OUT[i].real();
		outputSignals[0]->bufferPut((t_real)(val));
	}

	return true;
}



///// Overlap save::Transfer Function : 2018 - 11 - 10 //////
/////////////////////////////////////////////////////////////
////////////////////// FD_Filter_20181110 /////////////////// 
/////////////////////////////////////////////////////////////
void FD_Filter_20181110::initializeFD_Filter_20181110(void)
{
	outputSignals[0]->symbolPeriod = inputSignals[0]->symbolPeriod;
	outputSignals[0]->samplingPeriod = inputSignals[0]->samplingPeriod;
	outputSignals[0]->samplesPerSymbol = inputSignals[0]->samplesPerSymbol;

	if (!getSeeBeginningOfTransferFunction()) {
		int aux = (int)(((double)transferFunctionLength) / 4) + 1;
		outputSignals[0]->setFirstValueToBeSaved(aux);
	}

	if (saveTransferFunction)
	{
		ofstream fileHandler("./signals/" + transferFunctionFilename, ios::out);
		fileHandler << "// ### HEADER TERMINATOR ###\n";

		double samplingPeriod = inputSignals[0]->samplingPeriod;
		t_real fWindow = 1 / samplingPeriod;
		t_real df = fWindow / transferFunction.size();

		t_real f;
		for (int k = 0; k < transferFunction.size(); k++)
		{
			f = -fWindow / 2 + k * df;
			fileHandler << f << " " << transferFunction[k] << "\n";
		}
		fileHandler.close();
	}
}

bool FD_Filter_20181110::runBlock(void)
{
	bool alive{ false };

	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();
	int process = min(ready, space);
	if (process == 0) return false;

	//////////////////////  currentCopy //////////////////////////
	//////////////////////////////////////////////////////////////
	vector<t_complex> currentCopy(process); // Get the Input signal
	t_real input;
	for (int i = 0; i < process; i++) {
		inputSignals[0]->bufferGet(&input);
		currentCopy.at(i) = { input, 0};
	}
	
	vector<t_complex> pcinitialize(process); 
	if (K == 0)
	{
		if (symmetryTypeTf == "Symmetric")
			previousCopy = pcinitialize;

		else
			previousCopy = currentCopy;
	}

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

	vector<t_complex> out;
    out = overlapSaveTransferFunction(currentCopy,previousCopy,ifftshift(transferFunction));

	// Bufferput
	for (int i = 0; i < process; i++) {
		t_real val;
		val = out[i].real();
		outputSignals[0]->bufferPut((t_real)(val));
	}

	K = K + 1;
	previousCopy = currentCopy;

	return true;
}

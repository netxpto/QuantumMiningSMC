# include "netxpto.h"
#include"fft_20180208.h"
# include "overlap_save_20180208.h"

/////////////////////////////////////////////////////////////
//////////////////////// FIR_Filter ///////////////////////// TIME DOMAIN
/////////////////////////////////////////////////////////////
class FIR_Filter : public Block {

	// State Variable
	vector<t_real> delayLine;

	bool saveImpulseResponse{ true };
	string impulseResponseFilename{ "impulse_response.imp" };


	// Input Parameters 
	bool seeBeginningOfImpulseResponse{ true };

public:

	// State Variable 
	vector<t_real> impulseResponse;

	// Input Parameters
	int impulseResponseLength;				   


	// Methods 
	FIR_Filter() {};
	FIR_Filter(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initializeFIR_Filter(void);
	bool runBlock(void);
	void terminate(void) {};



	void setSaveImpulseResponse(bool sImpulseResponse) { saveImpulseResponse = sImpulseResponse; };
	bool getSaveImpulseResponse(void) { return saveImpulseResponse; };

	void setImpulseResponseLength(int iResponseLength) { impulseResponseLength = iResponseLength; };
	int const getImpulseResponseLength() { return impulseResponseLength; }

	void setSeeBeginningOfImpulseResponse(bool sBeginning) { seeBeginningOfImpulseResponse = sBeginning; };
	bool const getSeeBeginningOfImpulseResponse() { return seeBeginningOfImpulseResponse; };
};

////// Overlap save :: Impulse Response : 2018-03-06 ////////
/////////////////////////////////////////////////////////////
//////////////////////// FIR_Filter ///////////////////////// 
/////////////////////////////////////////////////////////////

class FD_Filter : public Block {
	
	// State Variable
	vector<t_complex> previousCopy;
	int K = 0;

	bool saveImpulseResponse{ true };
	string impulseResponseFilename{ "impulse_response.imp" };

	// Input Parameters 
	bool seeBeginningOfImpulseResponse{ true };

public:

	// State Variable 
	vector<t_real> impulseResponse;
	string symmetryTypeIr;

	// Input Parameters
	int impulseResponseLength;


	// Methods 
	FD_Filter() {};
	FD_Filter(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initializeFD_Filter(void);
	bool runBlock(void);
	void terminate(void) {};

	void setSaveImpulseResponse(bool sImpulseResponse) { saveImpulseResponse = sImpulseResponse; };
	bool getSaveImpulseResponse(void) { return saveImpulseResponse; };

	void setImpulseResponseLength(int iResponseLength) { impulseResponseLength = iResponseLength; };
	int const getImpulseResponseLength() { return impulseResponseLength; }

	void setSeeBeginningOfImpulseResponse(bool sBeginning) { seeBeginningOfImpulseResponse = sBeginning; };
	bool const getSeeBeginningOfImpulseResponse() { return seeBeginningOfImpulseResponse; };

};

///// Overlap save::Transfer Function : 2018 - 11 - 10 //////
/////////////////////////////////////////////////////////////
//////////////////////// FIR_Filter ///////////////////////// 
/////////////////////////////////////////////////////////////
class FD_Filter_20181110 : public Block {

	// State Variable
	vector<t_complex> previousCopy;
	int K{ 0 };
	int L{ 0 };
	bool saveTransferFunction{ true };
	string transferFunctionFilename{ "transfer_function.tfn" };

	// Input Parameters 
	bool seeBeginningOfTransferFunction{ true };

public:
	// State Variable
	vector<t_complex> transferFunction;
	string symmetryTypeTf;

	// Input parameter
	int transferFunctionLength;

	/* Methods */
	FD_Filter_20181110() {};
	FD_Filter_20181110(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initializeFD_Filter_20181110(void);
	bool runBlock(void);
	void terminate(void) {};

	void setSaveTransferFunction(bool sTransferFunction) { saveTransferFunction = sTransferFunction; };
	bool getSaveTransferFunction(void) { return saveTransferFunction; };

	void setSeeBeginningOfTransferFunction(bool sBeginning) { seeBeginningOfTransferFunction = sBeginning; };
	bool const getSeeBeginningOfTransferFunction() { return seeBeginningOfTransferFunction; };

	void setTransferFunctionLength(int iTransferFunctionLength) { transferFunctionLength = iTransferFunctionLength; };
	int const getTransferFunctionLength() { return transferFunctionLength; };

};


#pragma once

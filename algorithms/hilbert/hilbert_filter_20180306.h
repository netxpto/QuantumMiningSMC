#pragma once
# ifndef HILBERT_FILTER_20180306_H_
# define HILBERT_FILTER_20180306_H_

# include "netxpto.h"

using namespace std;


class HilbertTransform : public Block {

public:
	// State Variable
	vector<t_complex> transferFunction;

	// Input parameter
	int transferFunctionLength;

	/* Methods */
	HilbertTransform() {};
	HilbertTransform(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);
	void terminate(void) {};

	void setTransferFunctionFrequencyLength(int transFunctionLength) { transferFunctionFrequencyLength = transFunctionLength; };
	int const getTransferFunctionFrequencyLength(void) { return transferFunctionFrequencyLength; };

	void setSaveTransferFunction(bool sTransferFunction) { saveTransferFunction = sTransferFunction; };				    // saveTransferFunction
	bool getSaveTransferFunction(void) { return saveTransferFunction; };

	void setSeeBeginningOfTransferFunction(bool sBeginning) { seeBeginningOfTransferFunction = sBeginning; };			// seeBeginningOfTransferFunction
	bool const getSeeBeginningOfTransferFunction() { return seeBeginningOfTransferFunction; };

	void setTransferFunctionLength(int iTransferFunctionLength) { transferFunctionLength = iTransferFunctionLength; };  // transferFunctionLength
	int const getTransferFunctionLength() { return transferFunctionLength; };


private:
	// State Variable
	vector<t_complex> previousCopy1;
	vector<t_complex> previousCopy2;
	int K{ 0 };
	int L{ 0 };
	bool saveTransferFunction{ true };
	string transferFunctionFilename{ "transfer_function.tfn" };

	// Input Parameters 
	bool seeBeginningOfTransferFunction{ true };
	int transferFunctionFrequencyLength{ 16 };


};



/*class HilbertFilter : public HilbertRoot {

	// Input Parameters
	int transferFunctionFrequencyLength{ 16 };
	bool passiveFilterMode{ false };
	string transferFunctionFilename{"hilbert_transformer.tfn"};
public:

	// Methods 
	HilbertFilter() : HilbertRoot() {};
	HilbertFilter(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :HilbertRoot(InputSig, OutputSig) {};

	void initialize(void);

	void setTransferFunctionFrequencyLength(int transFunctionLength) { transferFunctionFrequencyLength = transFunctionLength; };
	int const getTransferFunctionFrequencyLength(void) { return transferFunctionFrequencyLength; };
	
	void usePassiveFilterMode(bool pFilterMode) { passiveFilterMode = pFilterMode; };

};*/

# endif



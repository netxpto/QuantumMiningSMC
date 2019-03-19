
# ifndef HILBERT_FILTER_H
# define HILBERT_FILTER_H
# include <vector>
# include "netxpto.h"

using namespace std;

class HilbertRoot : public Block {
	/* State Variable */
	vector<t_complex> previousCopy;
	int K{ 0 };
	bool saveTransferFunction{ true };
	string transferFunctionFilename{ "transfer_function.tfn" };
	bool seeBeginningOfTransferFunction{ true };

public:
	/* State Variable */
	vector<t_complex> transferFunction;
	int transferFunctionLength;


	/* Methods */
	HilbertRoot() {};
	HilbertRoot(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initialize_HilbertRoot(void);

	bool runBlock(void);

	void terminate(void) {};

	void setSaveTransferFunction(bool sTransferFunction) { saveTransferFunction = sTransferFunction; };
	bool getSaveTransferFunction(void) { return saveTransferFunction; };

	void setTransferFunctionLength(int iTransferFunctionLength) { transferFunctionLength = iTransferFunctionLength; };
	int const getTransferFunctionLength() { return transferFunctionLength; };

	void setSeeBeginningOfTransferFunction(bool sBeginning) { seeBeginningOfTransferFunction = sBeginning; };
	bool const getSeeBeginningOfTransferFunction() { return seeBeginningOfTransferFunction; };

};



class HilbertTransform : public HilbertRoot
{
	int transferFunctionFrequencyLength{ 16 };		// in units of symbol period
	string filterDomainType{ "frequency" };

public:
	HilbertTransform() : HilbertRoot() {};
	HilbertTransform(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :HilbertRoot(InputSig, OutputSig) {};

	void initialize(void);

	void setTransferFunctionFrequencyLength(int transFunctionFrequencyLength) { transferFunctionFrequencyLength = transFunctionFrequencyLength; };
	int const getTransferFunctionLength(void) { return transferFunctionFrequencyLength; };

	void setFilterDomainType(string fDomain) { filterDomainType = fDomain; };
	string getFilterDomainType() { return filterDomainType; };

};


#endif
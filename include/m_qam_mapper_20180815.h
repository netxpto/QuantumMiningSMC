# ifndef M_QAM_MAPPER_H_
# define M_QAM_MAPPER_H_

# include "..\include\netxpto_20180815.h"


class MQamMapper : public Block {

public:

	//##############################################################################################################

	MQamMapper(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};
	//MQamMapper(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};
	//MQamMapper() {};

	void initialize(void);
	bool runBlock(void);

	//##############################################################################################################

	void setM(int mValue);		// m should be of the form m = 2^n, with n integer;
	void setIqAmplitudes(vector<vector<t_real>> iqAmplitudesValues);

	void setFirstTime(bool fTime) { firstTime = fTime; };  // if set to true the first output sample is going to be zero, to make the system causal
	bool getFirstTime() { return firstTime; };

	//##############################################################################################################

private:

	/* State Variables */

	t_integer auxBinaryValue{ 0 };
	t_integer auxSignalNumber{ 0 };
	bool firstTime{ true };

	/* Input Parameters */
	t_integer m{ 4 };
	vector<vector<t_real>> iqAmplitudes{ { 1, 1 },{ -1, 1 },{ -1, -1 },{ 1, -1 } };

};

#endif

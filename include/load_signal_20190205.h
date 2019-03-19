# ifndef PROGRAM_LOAD_SIGNAL_H_
# define PROGRAM_LOAD_SIGNAL_H_

# include "netxpto_20180815.h"


class LoadSignal : public Block {

public:

	LoadSignal(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setSgnFileName(string sFileName) { sgnFileName = sFileName; };
	string const getSGNFileName(void) { return sgnFileName; };


private:

	bool endOfFile{ false };

	int count = 0;
	int headerLength = 0;
	string sgnFileName{ "InputFile.sgn" };

};

# endif
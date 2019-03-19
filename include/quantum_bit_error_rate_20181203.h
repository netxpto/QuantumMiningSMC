# ifndef QUANTUM_BIT_ERROR_RATE_H
# define QUANTUM_BIT_ERROR_RATE_H

# include "../include/netxpto_20180815.h"

class QuantumBitErrorRate : public Block {

public:
	QuantumBitErrorRate(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	void setConfidence(double P) { alpha = 1 - P; }
	double const getConfidence(void) { return 1 - alpha; }

	void setDataSequence(string dSequence) { dataSequence = dSequence; };
	string getDataSequence() { return dataSequence; };

	void setWindow(long int nSamples) { window = nSamples; };
	long int getWindow() { return window; };

private:
//#############Input parameters####################################################################
	long int window{ -1 };
	double alpha = 0.05;
	string dataSequence{ "11" };

//#############System variables#####################################################################
	double z;
	int firstPass{ 1 };
	double coincidences = 0.0;
	double receivedBits = 0.0;
	int posDataSeq{ 0 };
	int n{ 0 };
	t_real QBER{ 0.0 };
};

#endif

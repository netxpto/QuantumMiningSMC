# ifndef PROGRAM_INCLUDE_DECISION_CIRCUIT_H_
# define PROGRAM_INCLUDE_DECISION_CIRCUIT_H_

# include "netxpto_20180418.h"


// Creates two real signals from a complex signal
class DecisionCircuitMQAM : public Block {

 public:

	/* Methods */
	 DecisionCircuitMQAM() {};
	 DecisionCircuitMQAM(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :
		Block(InputSig, OutputSig){};

	void initialize(void);
	bool runBlock(void);

	void setmQAM(int mQAMs) { mQAM = mQAMs; }
	double getmQAM() { return mQAM; }

private:

	/* State Variables */
	bool firstTime{ true };

	/* Input Parameters */
	int mQAM{ 4 };
};

# endif // PROGRAM_INCLUDE_DECISION_CIRCUIT_H_

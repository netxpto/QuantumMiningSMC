# ifndef PROGRAM_INCLUDE_MATH_OPERATIONS_H_
# define PROGRAM_INCLUDE_MATH_OPERATIONS_H_

# include "netxpto.h"

enum MathOperationType { Sqrt, NaturalLog, Exponential };

class Math : public Block {

	bool firstTime{ true };

public:

	Math() {};
	Math(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	
	void setMathOpertationType(MathOperationType fType) { operationType = fType; };
	MathOperationType const getMathOpertationType(void) { return operationType; };

private:
	MathOperationType operationType { Sqrt };
};

#endif
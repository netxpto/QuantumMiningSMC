# ifndef PROGRAM_INCLUDE_SOURCE_CODE_EFFICIENCY_H_
# define PROGRAM_INCLUDE_SOURCE_CODE_EFFICIENCY_H_

# include "netxpto_20180418.h"
# include <vector>

// Takes a sampling of the input electrical signal
class SourceCodeEfficiency : public Block {

public:
	void initialize(void);
	bool runBlock(void);

	void setProbabilityOfZero(double s_n);
	double getProbabilityOfZero();
	void setSourceOrder(int s_n);
	int getSourceOrder();

	SourceCodeEfficiency() {};
	SourceCodeEfficiency(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

private:
	double probabilityOfZero = 0.05; // = 0.7
	int sourceOrder = 2; // = 2
	int totalSymbols = 0;

	/* Order 2 */
	double probability_X2[4] = { 0, 0, 0, 0 };
	int counter_X2[4] = { 0, 0, 0, 0 };
	double length_X2[4] = { 0, 0, 0, 0 };
	double entropy_X2[4] = { 0, 0, 0, 0 };
	int symbolLength_X2[4] = { 3, 3, 2, 1 };
	/* Order 3 */
	double probability_X3[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int counter_X3[8] = { 0, 0, 0, 0, 0, 0, 0, 0};
	double length_X3[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	double entropy_X3[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int symbolLength_X3[8] = { 7, 7, 6, 5, 4, 3, 2, 1 };
	/* Order 4 */
	double probability_X4[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int counter_X4[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double length_X4[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	double entropy_X4[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int symbolLength_X4[16] = { 15, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

	bool getData(int counter);
	void efficiencyCalc(void);
};

#endif // !PROGRAM_INCLUDE_SOURCE_CODE_EFFICIENCY_H_

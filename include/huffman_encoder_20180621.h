# ifndef PROGRAM_INCLUDE_HUFFMAN_ENCODER_H_
# define PROGRAM_INCLUDE_HUFFMAN_ENCODER_H_

# include "netxpto_20180418.h"
# include <vector>

// Takes a sampling of the input electrical signal
class HuffmanEncoder : public Block {

public:
	void initialize(void);
	bool runBlock(void);

	void setProbabilityOfZero(double s_n);
	double getProbabilityOfZero();
	void setSourceOrder(int s_n);
	int getSourceOrder();

	HuffmanEncoder() {};
	HuffmanEncoder(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

private:
	double probabilityOfZero = 0.05;
	int sourceOrder = 2;

};

#endif // !PROGRAM_INCLUDE_HUFFMAN_ENCODER_H_

# ifndef PROGRAM_INCLUDE_HUFFMAN_DECODER_H_
# define PROGRAM_INCLUDE_HUFFMAN_DECODER_H_

# include "netxpto_20180418.h"
# include <vector>

// Takes a sampling of the input electrical signal
class HuffmanDecoder : public Block {

public:
	void initialize(void);
	bool runBlock(void);

	void setProbabilityOfZero(double s_n);
	double getProbabilityOfZero();
	void setSourceOrder(int s_n);
	int getSourceOrder();

	HuffmanDecoder() {};
	HuffmanDecoder(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

private:
	double probabilityOfZero = 0.05;
	int sourceOrder = 2 ;
	bool getData(int counter);
};

#endif // !PROGRAM_INCLUDE_HUFFMAN_DECODER_H_

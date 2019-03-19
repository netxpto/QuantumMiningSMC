# include "netxpto_20180418.h"
# include <bitset>

class BinaryToAscii : public Block {

private:
	int bitIndex{ 0 };
	char aux{'\0'}; // Equivalent to: 0000 0000

public:
	BinaryToAscii() {};
	BinaryToAscii(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) : Block(InputSig, OutputSig) { };

	void initialize(void);

	bool runBlock(void);

};
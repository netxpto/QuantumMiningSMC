# include "netxpto_20180418.h"
# include <bitset>

using namespace std;

class AsciiToBinary : public Block {

private:
	int bitIndex{ 0 }; //The bit of the next character to be analysed
	char aux{ '\0' };

public:
	AsciiToBinary() {};
	AsciiToBinary(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) : Block(InputSig, OutputSig) { };

	void initialize(void);

	bool runBlock(void);

};
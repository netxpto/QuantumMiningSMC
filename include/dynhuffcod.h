# ifndef PROGRAM_INCLUDE_DYNHUFFCOD_H_
# define PROGRAM_INCLUDE_DYNHUFFCOD_H_

#include "netxpto_20180418.h"

struct HuffmanTree {
	char c;
	int cfreq;
	int id;
	struct HuffmanTree *left;
	struct HuffmanTree *right;

	HuffmanTree(char c, int cfreq, struct HuffmanTree *left = NULL, struct HuffmanTree *right = NULL, int id = -1) :
		id(id), c(c), cfreq(cfreq), left(left), right(right) {
	}

	~HuffmanTree() {
		delete left, delete right;
	}
};

class DynHuffmanCod : public Block {

	bool firstTime{ true };

public:

	DynHuffmanCod() {};
	DynHuffmanCod(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);
	
private:

	HuffmanTree *root = new HuffmanTree(0, 0);
	HuffmanTree *empty = new HuffmanTree(0, -1);
};


#endif _

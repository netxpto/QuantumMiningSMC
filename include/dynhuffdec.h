#ifndef PROGRAM_INCLUDE_DYNHUFFDEC_H_
#define PROGRAM_INCLUDE_DYNHUFFDEC_H_

#include "netxpto_20180418.h"
#include "dynhuffcod.h"

class DynHuffmanDec : public Block {

	bool firstTime{ true };

public:

	DynHuffmanDec() {};
	DynHuffmanDec(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};
	
	void initialize(void);
	bool runBlock(void);
	
private:

	HuffmanTree * root = new HuffmanTree(0, 0);
	HuffmanTree *empty = new HuffmanTree(0, -1);

};

#endif _

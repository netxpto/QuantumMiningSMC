#include <algorithm>
#include <complex>
#include <fstream>
#include <bitset>
#include "netxpto_20180418.h"
#include "dynhuffdec.h"
#include "dynhuffcod.h"

int ident2 = 0;

bool incrementNode(char cin, HuffmanTree *root, int i);
bool insertNode(char cin, HuffmanTree *root, int i);
int getIntoTree(char* out, HuffmanTree *root, vector<Signal *> &InputSig);
void sortTree2(HuffmanTree *root);
void obtainList2(HuffmanTree *root, int *list, int *identList, int *deepth, char *node, int i);
void exchangeNodes2(HuffmanTree *root, int freqI, int freqJ, char i, char j, int idI, int idJ);
int searchNode2(HuffmanTree *root, int i, HuffmanTree *node, char ci, int freqI, int freqJ, int op, int idI);

void DynHuffmanDec::initialize(void) {
	root->left = empty;
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
}

bool DynHuffmanDec::runBlock(void) {
	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	if (ready == 0 && space == 0) return false;
	int out[8];

	while (ready > 0 && space > 0) {
		
		char input = 0;
		int i = 0;
		int sel = getIntoTree(&input, root, inputSignals);
		
		if (sel == 1) {
			incrementNode(input, root, i);
			std::bitset<8> output(input);
			for (int k = 0; k < 8; k++) {
				out[k]=(int)output[k];
			}
		}

		if (sel == 2) {
			for (i = 0; i < 8; ++i) {
				inputSignals[0]->bufferGet(&out[i]);
				input = (int)input + (int)(out[i] * pow(2, i));
			}
			insertNode(input, root, i);
		}

		sortTree2(root);

		for (i = 0; i < 8; ++i) {
			outputSignals[0]->bufferPut(out[i]);
		}

		ready = inputSignals[0]->ready();
		space = outputSignals[0]->space();
	}

	return false;
}

int getIntoTree(char* out, HuffmanTree *root, vector<Signal *> &InputSignals) {
	
	if ((root->right) == NULL && (root->left->cfreq == -1))
		return 2;

	while(1){
		if (root->cfreq == -1)
			return 2;
		if ((root->right == NULL) && (root->left == NULL)) {
			*out = root->c;
			return 1;
		}

		int in;
		InputSignals[0]->bufferGet(&in);

		if (in == 0)
			root = root->left;

		if (in == 1)
			root = root->right;

	}
}

bool incrementNode(char cin, HuffmanTree *root, int i) {
	
	if (root->c == cin) {
		root->cfreq++;
		return true;
	}

	if (root->left != NULL) {
		if (incrementNode(cin, root->left, i + 1)) {
			if (i != 0) root->cfreq++;
			return true;
		}
	}

	if (root->right != NULL) {
		if (incrementNode(cin, root->right, i + 1)) {
			if (i != 0) root->cfreq++;
			return true;
		}
	}

	return false;
}

bool insertNode(char cin, HuffmanTree *root, int i) {
	
	if (root->left != NULL) {
		if ((root->left->cfreq == -1) && (root->right == NULL)) {
			HuffmanTree *newNode = new HuffmanTree(cin, 1);
			root->right = newNode;

			return true;
		}
	}

	if (root->cfreq == -1) {
		HuffmanTree *newNode = new HuffmanTree(cin, 1);
		HuffmanTree *empt = new HuffmanTree(0, -1);
		root->right = newNode;
		root->left = empt;
		root->cfreq = 1;
		root->id = ident2;
		ident2++;

		return true;
	}

	if (root->left != NULL) {
		if (insertNode(cin, root->left, i + 1)) {
			if (i != 0) root->cfreq++;
			return true;
		}
	}

	if (root->right != NULL) {
		if (insertNode(cin, root->right, i + 1)) {
			if (i != 0) root->cfreq++;
			return true;
		}
	}

	return false;
}

void sortTree2(HuffmanTree *root) {

	bool notOrdered = true;

	while (notOrdered) {
		notOrdered = false;

		int j = 0, i = 0, dpth = 0, idx = 0;
		int list[128], deepth[128], orderedList[128], identList[128], orderedidentList[128];
		char node[128], orderedNode[128];

		for (j = 0; j < 128; j++) {
			list[j] = -1;
			deepth[j] = -1;
			identList[j] = -1;
			node[j] = 0;
			orderedList[j] = 10000;
			orderedNode[j] = 0;
		}

		obtainList2(root, list, identList, deepth, node, i);

		for (dpth = 128; dpth >= 0; dpth--) {
			for (i = 0; i < 128; i++) {
				if (deepth[i] == dpth) {
					orderedList[idx] = list[i];
					orderedNode[idx] = node[i];
					orderedidentList[idx] = identList[i];
					idx++;
				}
			}
		}

		for (i = 0; i < 127; i++) {
			if (orderedList[i] > orderedList[i + 1]) {
				j = i + 1;
				notOrdered = true;

				int auxi = orderedList[i];
				int auxj = orderedList[i + 1];

				while (auxi == orderedList[i]) {
					i--;
				}i++;

				while (auxj == orderedList[j]) {
					j++;
				}j--;

				break;
			}
		}

		if (notOrdered) exchangeNodes2(root, orderedList[i], orderedList[j], orderedNode[i], orderedNode[j], orderedidentList[i], orderedidentList[j]);

	}
}


void obtainList2(HuffmanTree *root, int *list, int *identList, int *deepth, char *node, int i) {

	int j = 0;
	for (j = 0; j < 128; j++) {
		if (deepth[j] == -1)
			break;
	}

	if (i != 0) {
		list[j] = root->cfreq;
		node[j] = root->c;
		deepth[j] = i;
		identList[j] = root->id;
	}

	if (root->left != NULL)
		obtainList2(root->left, list, identList, deepth, node, i + 1);

	if (root->right != NULL)
		obtainList2(root->right, list, identList, deepth, node, i + 1);

}

void exchangeNodes2(HuffmanTree *root, int freqI, int freqJ, char ci, char cj, int idI, int idJ) {

	int i = 0;
	HuffmanTree *NodeI = new HuffmanTree(0, 0);
	HuffmanTree *NodeJ = new HuffmanTree(0, 0);
	HuffmanTree *aux = new HuffmanTree(0, 0);

	int selI = searchNode2(root, i, NodeI, ci, freqI, (freqI - freqJ), -1, idI);
	int selJ = searchNode2(root, i, NodeJ, cj, freqJ, (freqI - freqJ), 1, idJ);

	if (selI == 1 && selJ == 1) {
		aux->right = NodeI->right->left;
		NodeI->right->left = NodeJ->right->left;
		NodeJ->right->left = aux->right;
	}

	if (selI == 1 && selJ == 2) {
		aux->right = NodeI->right->left;
		NodeI->right->left = NodeJ->right->right;
		NodeJ->right->right = aux->right;
	}

	if (selI == 2 && selJ == 1) {
		aux->right = NodeI->right->right;
		NodeI->right->right = NodeJ->right->left;
		NodeJ->right->left = aux->right;
	}

	if (selI == 2 && selJ == 2) {
		aux->right = NodeI->right->right;
		NodeI->right->right = NodeJ->right->right;
		NodeJ->right->right = aux->right;
	}

}

int searchNode2(HuffmanTree *root, int i, HuffmanTree *node, char ci, int freqI, int freqDiff, int op, int idI) {

	if ((root->c == ci) && (root->cfreq == freqI) && (root->id == idI)) {
		return 3;
	}

	if (root->left != NULL) {
		int sel = searchNode2(root->left, i + 1, node, ci, freqI, freqDiff, op, idI);
		if (sel != 0) {
			if (i != 0) root->cfreq = root->cfreq + freqDiff * op;
			if (sel == 3) {
				node->right = root;
				return 1;
			}
			if (sel == 1) return 1;
			if (sel == 2) return 2;
		}
	}
	if (root->right != NULL) {
		int sel = searchNode2(root->right, i + 1, node, ci, freqI, freqDiff, op, idI);
		if (sel != 0) {
			if (i != 0) root->cfreq = root->cfreq + freqDiff * op;
			if (sel == 3) {
				node->right = root;
				return 2;
			}
			if (sel == 1) return 1;
			if (sel == 2) return 2;
		}
	}

	return 0;
}

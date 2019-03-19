#include <algorithm>
#include <complex>
#include <fstream>
#include <bitset>
#include "netxpto_20180418.h"
#include "dynhuffcod.h"

bool existsNode(char cin, HuffmanTree *root);
bool incrementNode(char cin, int* out, HuffmanTree *root, int i);
bool insertNode(char cin, int* out, HuffmanTree *root, int i, int* in);
void sortTree(HuffmanTree *root);
void obtainList(HuffmanTree *root, int *list, int *identList, int *deepth, char *node, int i);
void exchangeNodes(HuffmanTree *root, int freqI, int freqJ, char i, char j, int idI, int idJ);
int searchNode(HuffmanTree *root, int i, HuffmanTree *node, char ci, int freqI, int freqJ, int op, int idI);

int ident = 0;

void DynHuffmanCod::initialize(void){
	if (firstTime) {
		root->left = empty;

		outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
		outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
		outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());
	}
	firstTime = false;
}


bool DynHuffmanCod::runBlock(void){
	int ready = inputSignals[0]->ready();
	int space = outputSignals[0]->space();

	if (ready == 0 && space == 0) return false;

	int i = 0;
	int in[8], out[136];
	char input = 0;

	for (i = 0; i < 136; ++i)
		out[i] = -1;

	while (ready > 0 && space > 136) {
		input = 0;
		for (i = 0; i < 8; ++i) {
			inputSignals[0]->bufferGet(&in[i]);
			input = (int)input + (int)(in[i]*pow(2, i));
		}

		i = 0;
		if (existsNode(input, root))
			incrementNode(input, out, root, i);
		else {
			insertNode(input, out, root, i, in);
		}

		sortTree(root);

		for (i = 0; i < 136; ++i) {
			if (out[i] != -1) {
				outputSignals[0]->bufferPut(out[i]);
				out[i] = -1;
			}
		}

		ready = inputSignals[0]->ready();
		space = outputSignals[0]->space();
	}
	
	return false;
}

bool existsNode(char cin, HuffmanTree *root) {

	if (root->c == cin)
		return true;

	if (root->left != NULL) {
		if (existsNode(cin, root->left))
			return true;
	}

	if (root->right != NULL) {
		if (existsNode(cin, root->right))
			return true;
	}

	return false;

}

bool incrementNode(char cin, int* out, HuffmanTree *root, int i) {
		
	if (root->c == cin) {
		root->cfreq++;
		return true;
	}

	if (root->left != NULL) {
		if (incrementNode(cin, out, root->left, i + 1)) {
			if(i!=0) root->cfreq++;
			out[i] = 0;
			return true;
		}
	}

	if (root->right != NULL) {
		if (incrementNode(cin, out, root->right, i + 1)) {
			if (i != 0) root->cfreq++;
			out[i] = 1;
			return true;
		}
	}

	return false;
}

bool insertNode(char cin, int* out, HuffmanTree *root, int i, int* in) {
	
	if (root->left != NULL) {
		if ((root->left->cfreq == -1) && (root->right == NULL)) {
			HuffmanTree *newNode = new HuffmanTree(cin, 1);
			root->right = newNode;

			int k = 0;
			for (int j = i; j < i + 8; j++) {
				out[j] = in[k];
				k++;
			}

			return true;
		}
	}

	if (root->cfreq == -1) {
		HuffmanTree *newNode = new HuffmanTree(cin, 1);
		HuffmanTree *empt = new HuffmanTree(0, -1);
		root->right = newNode;
		root->left = empt;
		root->cfreq = 1;
		root->id = ident;
		ident++;

		int k = 0;
		for (int j = i; j < i + 8; j++) {
			out[j] = in[k];
			k++;
		}

		return true;
	}

	if (root->left != NULL) {
		if (insertNode(cin, out, root->left, i + 1, in)) {
			if (i != 0) root->cfreq++;
			out[i] = 0;
			return true;
		}
	}

	if (root->right != NULL) {
		if (insertNode(cin, out, root->right, i + 1, in)) {
			if (i != 0) root->cfreq++;
			out[i] = 1;
			return true;
		}
	}

	return false;

}

void sortTree(HuffmanTree *root) {
	
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

		obtainList(root, list, identList, deepth, node, i);

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
				int auxj = orderedList[i+1];

				while(auxi == orderedList[i]) {
					i--;
				}i++;

				while (auxj == orderedList[j]) {
					j++;
				}j--;

				break;
			}
		}

		if(notOrdered) exchangeNodes(root, orderedList[i], orderedList[j], orderedNode[i], orderedNode[j], orderedidentList[i], orderedidentList[j]);

	}
}


void obtainList(HuffmanTree *root, int *list, int *identList, int *deepth, char *node, int i) {

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
		obtainList(root->left, list, identList, deepth, node, i + 1);

	if (root->right != NULL)
		obtainList(root->right, list, identList, deepth, node, i + 1);

}

void exchangeNodes(HuffmanTree *root, int freqI, int freqJ, char ci, char cj, int idI, int idJ) {
	
	int i = 0;
	HuffmanTree *NodeI = new HuffmanTree(0, 0);
	HuffmanTree *NodeJ = new HuffmanTree(0, 0);
	HuffmanTree *aux = new HuffmanTree(0, 0);

	int selI = searchNode(root, i, NodeI, ci, freqI, (freqI - freqJ), -1, idI);
	int selJ = searchNode(root, i, NodeJ, cj, freqJ, (freqI - freqJ), 1, idJ);
	
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

int searchNode(HuffmanTree *root, int i, HuffmanTree *node, char ci, int freqI, int freqDiff, int op, int idI) {

		if ((root->c == ci) && (root->cfreq == freqI) && (root->id == idI)) {
			return 3;
		}

		if (root->left != NULL) {
			int sel = searchNode(root->left, i + 1, node, ci, freqI, freqDiff, op, idI);
			if(sel != 0){
				if(i != 0) root->cfreq = root->cfreq + freqDiff * op;
				if (sel == 3) {
					node->right = root;
					return 1;
				}
				if (sel == 1) return 1;
				if (sel == 2) return 2;
			}
		}
		if (root->right != NULL) {
			int sel = searchNode(root->right, i + 1, node, ci, freqI, freqDiff, op, idI);
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
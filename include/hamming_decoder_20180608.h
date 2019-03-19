# ifndef PROGRAM_INCLUDE_HAMMING_DECODER_H_
# define PROGRAM_INCLUDE_HAMMING_DECODER_H_

# include "netxpto_20180418.h"
# include <vector>

// Takes a sampling of the input electrical signal
class HammingDeCoder : public Block {

public:
	void initialize(void);
	bool runBlock(void);

	void setNBits(int s_n);
	int getNBits();
	void setKBits(int s_n);
	int getKBits();

	HammingDeCoder() {};
	HammingDeCoder(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

private:
	int parityBits = 3;		/* Default: Hamming Code (7, 4) */
	int nBits = 7;			/* Default: Code word, N = 7 */
	int kBits = 4;			/* Default: Data, K = 4 */

	int k_array[7] = { 1, 4, 11, 26, 57, 120, 247 };
	int n_array[7] = { 3, 7, 15, 31, 63, 127, 255 };
	int parity_array[7] = { 2, 3, 4, 5, 6, 7, 8 };
	vector<vector<int>> H;

	void setHMatrix(int s_n);
	void setParityBits(void);
	int getParityBits(void);
};

#endif // !PROGRAM_INCLUDE_HAMMING_DECODER_H_

# ifndef PROGRAM_INCLUDE_ARITHMETIC_ENCODER_H_
# define PROGRAM_INCLUDE_ARITHMETIC_ENCODER_H_

# include "netxpto_20180118.h"
# include <vector>


class ArithmeticEncoder : public Block {

private:
	unsigned int			seq_len;
	unsigned int			bits_per_symb;
	unsigned long			lim_low, lim_high;
	unsigned long			lim_min, lim_max;
	unsigned long			delta;
	unsigned int			num_extra_bits;
	unsigned int			total_count, min_count;
	unsigned int			resolution;
	unsigned int			code_bit;
	unsigned int			num_symbols_coded;
	bool					coding_complete;
	vector<unsigned int>    src_counts, src_cum_counts;
	vector<unsigned int>    code_buff;

public:

	void initialize(void);
	bool runBlock(void);

	ArithmeticEncoder() {};
	ArithmeticEncoder(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) : Block(InputSig, OutputSig) {};

	void init(const unsigned int& SeqLen, const unsigned int& BitsPerSymb, const vector<unsigned int>& SrcCounts);

};


#endif // !PROGRAM_INCLUDE_ARITHMETIC_ENCODER_H_

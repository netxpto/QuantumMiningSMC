# ifndef PROGRAM_INCLUDE_ARITHMETIC_DECODER_H_
# define PROGRAM_INCLUDE_ARITHMETIC_DECODER_H_

# include "netxpto_20180118.h"
# include <vector>


class ArithmeticDecoder : public Block {

private:
	unsigned int			seq_len;
	unsigned int			bits_per_symb;
	unsigned long			lim_low, lim_high;
	unsigned long			lim_min, lim_max;
	unsigned long			delta;
	unsigned int			total_count, min_count;
	unsigned int			resolution;
	unsigned int			tag, tag_norm, out_symb;
	unsigned int			num_symbols_decoded;
	bool					decoding_complete, init_decoding;
	vector<unsigned int>    src_counts, src_cum_counts;
	vector<unsigned int>    decode_buff;

public:

	void initialize(void);
	bool runBlock(void);

	ArithmeticDecoder() {};
	ArithmeticDecoder(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) : Block(InputSig, OutputSig) {};

	void init(const unsigned int& SeqLen, const unsigned int& BitsPerSymb, const vector<unsigned int>& SrcCounts);

};


#endif // !PROGRAM_INCLUDE_SAMPLER_H_

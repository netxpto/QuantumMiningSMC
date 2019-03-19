#include <algorithm>
#include <complex>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "netxpto_20180118.h"
#include "arithmetic_encoder.h"

using namespace std;

void ArithmeticEncoder::initialize(void) {
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
}

bool ArithmeticEncoder::runBlock(void) {

	unsigned int ready = inputSignals[0]->ready();
	unsigned int space = outputSignals[0]->space();

	bool C1, C2;

	// Send leftover code bits to output
	while (code_buff.size() > 0 && space > 0) {
		outputSignals[0]->bufferPut(code_buff.at(0));
		code_buff.erase(code_buff.begin());
		space = outputSignals[0]->space();
	}

	// Check if coding is completed
	if (coding_complete)
		return false;

	// Continue coding
	while (num_symbols_coded < seq_len) {

		// Check if input buffer has symbols and if output buffer has space
		if (ready < bits_per_symb || space == 0)
			return false;
		
		// Read new symbol
		unsigned long symb = 0; unsigned int bit_in;
		for (unsigned int i = 0; i < bits_per_symb; i++) {
			inputSignals[0]->bufferGet(&bit_in);
			symb = (symb << 1) + bit_in;
		}
		ready = inputSignals[0]->ready();
		num_symbols_coded++;

		// Update intervals
		delta = lim_high - lim_low + 1;
		lim_high = lim_low + (unsigned) floor(delta * src_cum_counts.at(symb + 1) / total_count) - 1;
		lim_low = lim_low + (unsigned) floor(delta * src_cum_counts.at(symb) / total_count);

		// Scale interval
		C1 = (lim_low >> (resolution - 1)) == (lim_high >> (resolution - 1));
		C2 = (((lim_low >> (resolution - 2)) & 0x01) == 1) && (((lim_high >> (resolution - 2)) & 0x01) == 0);
		while (C1 || C2) {
			if (C1) {
				code_bit = (lim_low >> (resolution - 1)) & 0x01;
				code_buff.push_back(code_bit);
				for (; num_extra_bits > 0; num_extra_bits--)
					code_buff.push_back(code_bit ^ 0x01);

				lim_low = (lim_low << 1) & (lim_max - 1);
				lim_high = ((lim_high << 1) + 1) & (lim_max - 1);
			}
			else if (C2) {
				num_extra_bits++;
				lim_low = ((lim_low << 1) ^ (lim_max >> 1)) & (lim_max - 1);
				lim_high = (((lim_high << 1) ^ (lim_max >> 1)) & (lim_max - 1)) + 1;
			}

			C1 = (lim_low >> (resolution - 1)) == (lim_high >> (resolution - 1));
			C2 = (((lim_low >> (resolution - 2)) & 0x01) == 1) && (((lim_high >> (resolution - 2)) & 0x01) == 0);
		}

		// Send code bits to output
		space = outputSignals[0]->space();
		while (code_buff.size() > 0 && space > 0) {
			outputSignals[0]->bufferPut(code_buff.at(0));
			code_buff.erase(code_buff.begin());
			space = outputSignals[0]->space();
		}
	}

	space = outputSignals[0]->space();
	if (space >= (resolution + num_extra_bits) && code_buff.size() == 0) {

		code_bit = (lim_low >> (resolution - 1)) & 0x01;
		outputSignals[0]->bufferPut(code_bit);

		for (; num_extra_bits > 0; num_extra_bits--)
			outputSignals[0]->bufferPut(code_bit ^ 0x01);

		for (int i = resolution - 2; i >= 0; i--) {
			code_bit = (lim_low >> i) & 0x01;
			outputSignals[0]->bufferPut(code_bit);
		}
	}
	else {
		code_bit = (lim_low >> (resolution - 1)) & 0x01;
		code_buff.push_back(code_bit);

		for (; num_extra_bits > 0; num_extra_bits--)
			code_buff.push_back(code_bit ^ 0x01);

		for (int i = resolution - 2; i >= 0; i--) {
			code_bit = (lim_low >> i) & 0x01;
			code_buff.push_back(code_bit);
		}
	}

	coding_complete = true;

	ready = inputSignals[0]->ready();
	space = outputSignals[0]->space();

	return false;
}


void ArithmeticEncoder::init(const unsigned int& SeqLen, const unsigned int& BitsPerSymb,
						     const vector<unsigned int>& SrcCounts) {
	
	src_counts = SrcCounts;
	
	total_count = 0;
	src_cum_counts.reserve(src_counts.size());
	src_cum_counts.insert(src_cum_counts.begin(), total_count);
	min_count = *(src_counts.begin());
	for (std::vector<unsigned int>::iterator it = src_counts.begin(); it != src_counts.end(); ++it) {
		total_count += *it;		
		src_cum_counts.push_back(total_count);
		min_count = min(min_count, *it);
	}

	resolution = (unsigned int) ceil(log2(total_count / min_count))+1;

	lim_min = 0;
	lim_max = 1 << resolution;

	lim_low = lim_min;
	lim_high = lim_max - 1;
	delta = lim_high - lim_low + 1;

	seq_len = SeqLen;
	bits_per_symb = BitsPerSymb;
	num_symbols_coded = 0;
	num_extra_bits = 0;
	coding_complete = false;

}

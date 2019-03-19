#include <algorithm>
#include <complex>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "netxpto_20180118.h"
#include "arithmetic_decoder.h"

using namespace std;

void ArithmeticDecoder::initialize(void) {
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
}

bool ArithmeticDecoder::runBlock(void) {

	unsigned int ready = inputSignals[0]->ready();
	unsigned int space = outputSignals[0]->space();

	int bit_in;
	bool C1, C2;

	// Send all stored decoded symbols to output
	while (decode_buff.size() >= bits_per_symb && space >= bits_per_symb) {
		for (unsigned int i = 0; i < bits_per_symb; i++) {
			outputSignals[0]->bufferPut(decode_buff.at(0));
			decode_buff.erase(decode_buff.begin());
			space = outputSignals[0]->space();
		}
	}

	// Check if decoding is completed
	if (decoding_complete)
		return false;

	// If decoding has not yet started
	if (init_decoding) {

		// Check if there are enough input code bits
		if (ready < resolution) return false;

		init_decoding = false;

		// Read bits to initialize tag value
		for (unsigned int i = 0; i < resolution; i++) {
			inputSignals[0]->bufferGet(&bit_in);
			tag = (tag << 1) + bit_in;
		}
		ready = inputSignals[0]->ready();

		// Compute the normalized tag
		tag_norm = (unsigned int) floor(((tag - lim_low + 1) * total_count - 1) / (lim_high - lim_low + 1));

		// Search symbol
		unsigned int out_symb = 0;
		while (out_symb < (src_cum_counts.size()-1) && src_cum_counts.at(out_symb+1) <= tag_norm)
			out_symb++;
		num_symbols_decoded++;

		// Add symbol to decoded symbol buffer
		unsigned int out_bit;
		for (int i = bits_per_symb - 1; i >= 0; i--) {
			out_bit = (out_symb >> i) & 0x01;
			decode_buff.push_back(out_bit);
		}

		// Update intervals
		delta = lim_high - lim_low + 1;
		lim_high = lim_low + (unsigned int)floor(delta * src_cum_counts.at(out_symb + 1) / total_count) - 1;
		lim_low = lim_low + (unsigned int)floor(delta * src_cum_counts.at(out_symb) / total_count);
	}

	// Resume decoding
	while (num_symbols_decoded < seq_len) {

		// Scale interval
		C1 = (lim_low >> (resolution - 1)) == (lim_high >> (resolution - 1));
		C2 = (((lim_low >> (resolution - 2)) & 0x01) == 1) && (((lim_high >> (resolution - 2)) & 0x01) == 0);
		while (C1 || C2) {

			ready = inputSignals[0]->ready();
			space = outputSignals[0]->space();

			if (ready == 0) {
				// Send decoded symbols to output and return
				while (decode_buff.size() >= bits_per_symb && space >= bits_per_symb) {
					for (unsigned int i = 0; i < bits_per_symb; i++) {
						outputSignals[0]->bufferPut(decode_buff.at(0));
						decode_buff.erase(decode_buff.begin());
						space = outputSignals[0]->space();
					}
				}
				return false;
			}

			if (C1) {
				inputSignals[0]->bufferGet(&bit_in);
				tag = ((tag << 1) + bit_in) & (lim_max - 1);
				lim_low = (lim_low << 1) & (lim_max - 1);
				lim_high = ((lim_high << 1) + 1) & (lim_max - 1);
			}
			else if (C2) {
				inputSignals[0]->bufferGet(&bit_in);
				tag = (((tag << 1) + bit_in) ^ (lim_max >> 1)) & (lim_max - 1);
				lim_low = ((lim_low << 1) ^ (lim_max >> 1)) & (lim_max - 1);
				lim_high = (((lim_high << 1) ^ (lim_max >> 1)) & (lim_max - 1)) + 1;
			}

			C1 = (lim_low >> (resolution - 1)) == (lim_high >> (resolution - 1));
			C2 = (((lim_low >> (resolution - 2)) & 0x01) == 1) && (((lim_high >> (resolution - 2)) & 0x01) == 0);
		}

		// Update normalized tag
		tag_norm = (unsigned int) floor(((tag - lim_low + 1) * total_count - 1) / (lim_high - lim_low + 1));

		// Search symbol
		unsigned int out_symb = 0;
		while (out_symb <  (src_cum_counts.size() - 1) && src_cum_counts.at(out_symb+1) <= tag_norm)
			out_symb++;
		num_symbols_decoded++;

		// Add symbol to decoded symbol buffer
		unsigned int out_bit;
		for (int i = bits_per_symb - 1; i >= 0; i--) {
			out_bit = (out_symb >> i) & 0x01;
			decode_buff.push_back(out_bit);
		}

		// Update intervals
		delta = lim_high - lim_low + 1;
		lim_high = lim_low + (unsigned int)floor(delta * src_cum_counts.at(out_symb + 1) / total_count) - 1;
		lim_low = lim_low + (unsigned int)floor(delta * src_cum_counts.at(out_symb) / total_count);
	}

	// Send all stored decoded symbols to output
	space = outputSignals[0]->space();
	while (decode_buff.size() >= bits_per_symb && space >= bits_per_symb) {
		for (unsigned int i = 0; i < bits_per_symb; i++) {
			outputSignals[0]->bufferPut(decode_buff.at(0));
			decode_buff.erase(decode_buff.begin());
			space = outputSignals[0]->space();
		}
	}

	decoding_complete = true;

	ready = inputSignals[0]->ready();
	space = outputSignals[0]->space();

	return false;
}

void ArithmeticDecoder::init(const unsigned int& SeqLen, const unsigned int& BitsPerSymb,
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
	num_symbols_decoded = 0;
	tag = 0;
	tag_norm = 0;
	out_symb = 0;
	init_decoding = true;
	decoding_complete = false;
}

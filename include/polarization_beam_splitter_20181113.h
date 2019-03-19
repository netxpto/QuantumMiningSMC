# ifndef POLARIZATION_BEAM_SPLITTER_H_
# define POLARIZATION_BEAM_SPLITTER_H_

# include <algorithm>  // min()
# include <math.h>     // cos(), sin()
# include <array>

#include "../include/netxpto_20180815.h"


class PolarizationBeamSplitter : public Block {

public:

	//################################################################################################################################
	PolarizationBeamSplitter (initializer_list<Signal*> InputSig, initializer_list<Signal*> OutputSig) : Block(InputSig, OutputSig) {};
	PolarizationBeamSplitter(vector <Signal*> &inputSignals, vector <Signal*> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);
	bool runBlock(void);

	//################################################################################################################################
/*	void setTransferMatrixH(array<complex<double>, 4> TransferMatrix) { matrixH = TransferMatrix; }
	array<complex<double>, 4> const getTransferMatrixH(void) { return matrixH; }

	void setTransferMatrixV(array<complex<double>, 4> TransferMatrix) { matrixV = TransferMatrix; }
	array<complex<double>, 4> const getTransferMatrixV(void) { return matrixV; }
*/
	//##############################################################################################################

private:

	/* input parameters */
	t_real F = 1 / sqrt(2);
	t_complex unit = 1;
	array <t_complex, 4> matrix = { { F*unit, F*unit * 1i, F*unit * 1i, unit * F } };

	// For a 1:2 Polarization beamsplitter implementation bh and bv inputs are in vacuum state
	// only ah and av will be used to input the pbs
	t_complex ah = 0;
	t_complex bh = 0;
	t_complex av = 0;
	t_complex bv = 0;

	t_complex pbs[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0} };

	/*
		array <t_complex, 4> matrixH = { { 1, 0, 0, 0 } };
		array <t_complex, 4> matrixV = { { 0, 0, 0, 1 } };
	*/

};

#endif

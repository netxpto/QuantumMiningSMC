# ifndef POLARIZATION_BEAM_SPLITTER_H_
# define POLARIZATION_BEAM_SPLITTER_H_

# include <algorithm>  // min()
# include <math.h>     // cos(), sin()
# include <array>

#include "netxpto_20180418.h"


class PolarizationBeamSplitter : public Block {

public:

	PolarizationBeamSplitter(vector<Signal *> &inputSignals, vector<Signal *> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);

	bool runBlock(void);

	void setTransferMatrixH(array<complex<double>, 4> TransferMatrix) { matrixH = TransferMatrix; }
	array<complex<double>, 4> const getTransferMatrixH(void) { return matrixH; }

	void setTransferMatrixV(array<complex<double>, 4> TransferMatrix) { matrixV = TransferMatrix; }
	array<complex<double>, 4> const getTransferMatrixV(void) { return matrixV; }

private:

	/* input parameters */

	array <t_complex, 4> matrixH = { { 1, 0, 0, 0 } };
	array <t_complex, 4> matrixV = { { 0, 0, 0, 1 } };

};

#endif

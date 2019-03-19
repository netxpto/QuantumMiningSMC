# ifndef PROGRAM_INCLUDE_OPTICAL_HYBRID_H_
# define PROGRAM_INCLUDE_OPTICAL_HYBRID_H_

# include "netxpto_20180118.h"
# include <iostream>
# include <complex>
# include <fstream>
# include <algorithm>
# include <array>


// Implements a Balanced BeamSplitter
class OpticalHybrid : public Block {

	bool firstTime{ true };

public:

	t_complex div = 1 / 2;
	t_complex unit = 1;
	complex<t_real> imaginary=sqrt(-1);
	array <complex<double>, 8> matrix = { { 1 / 2, 1 / 2, 1 / 2, -1 / 2, 1 / 2, sqrt(-1) / 2, 1 / 2, -sqrt(-1) / 2 } };
	
	OpticalHybrid() {};
	OpticalHybrid(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};

	void initialize(void);
	bool runBlock(void);

	void setTransferMatrix(array<complex<double>, 8> TransferMatrix) { matrix = TransferMatrix; }
	array<complex<double>, 8> const getTransferMatrix(void) { return matrix; }

private:

};


#endif // !PROGRAM_INCLUDE_OPTICAL_HYBRID_H_

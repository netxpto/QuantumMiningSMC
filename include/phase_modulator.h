# ifndef PROGRAM_INCLUDE_PHASE_MODULATOR_H_
# define PROGRAM_INCLUDE_PHASE_MODULATOR_H_

# include "netxpto.h"
# include <vector>


// this is a test block for the purpose of beta testing new code
// current code: Phase Modulator.
class PhaseModulator : public Block {

	bool firstTime{ true };
	bool firstPass{ true };
	int bitsPerSymbol = 2;
	vector<t_binary> delay;
	vector<t_iqValues> constellation = { { 1, 1 } , { 1, -1 } , { -1, -1 } , { -1, 1 } };

public:

	PhaseModulator() {};
	PhaseModulator(vector<Signal*> &InputSig, vector<Signal*> &OutputSig) :Block(InputSig, OutputSig){};

	void initialize(void);
	bool runBlock(void);

	void setBitsPerSymbol(int bPerSymbol) { bitsPerSymbol = bPerSymbol; }
	int getBitsPerSymbol() { return bitsPerSymbol; }

	void setConstellation(vector<t_iqValues> ctellation) { constellation = ctellation; }
	vector<t_iqValues> getConstellation() { return constellation; }

};

#endif

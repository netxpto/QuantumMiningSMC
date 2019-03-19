# ifndef FRAMER_H_
# define FRAMER_H_

# include "../include/netxpto_20180815.h"

class QuantumFramer : public Block {

public:

	//##############################################################################################################

	QuantumFramer(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};
	//QuantumFramer(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) : Block(InputSig, OutputSig){};

	void initialize(void);
	bool runBlock(void);
	
	//##############################################################################################################

	void setFrameLength(t_integer fLength){ frameLength = fLength; };
	t_integer const getFrameLength(void) { return frameLength; };

	void setHeaderLength(t_integer hLength) { headerLength = hLength; };
	t_integer const getHeaderLength(void) { return headerLength; };

	//##############################################################################################################

private:

	/* Input Parameters */
	t_integer frameLength{ 0 };
	t_integer headerLength{ 0 };

	/* State Variables */
	t_integer bitPosition{ 0 };

};

# endif



# ifndef DELAY_H_
# define DELAY_H_

# include "../include/netxpto_20180815.h"

class TimeDelay : public Block {

public:

	//##################################################################################################################

	TimeDelay(initializer_list<Signal *> InputSig, initializer_list<Signal *> OutputSig) :Block(InputSig, OutputSig) {};
	//TimeDelay(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) : Block(InputSig, OutputSig){};

	void initialize(void);
	bool runBlock(void);
	
	//##################################################################################################################

	void setTimeDelay(t_real tDelay){ timeDelay = tDelay; };
	t_real const getTimeDelay(void) { return timeDelay; };

	//##################################################################################################################

private:

	/* Input Parameters */
	t_real timeDelay{ 0.0 };
};

# endif



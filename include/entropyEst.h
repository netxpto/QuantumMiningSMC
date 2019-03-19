#pragma once
# include <algorithm>   // std::min
# include <string>		// compare

# include "netxpto_20180118.h"

class entropyEst : public Block {

	int window;

public:
	
	long int numberOfSamples{ -1 };
	int w;

	//Methods
	entropyEst(){};
	entropyEst(vector<Signal *> &InputSig, int window);

	void setWindow(int w) { window = w; };
	int getWindow() { return window; };

	bool runBlock(void);


};


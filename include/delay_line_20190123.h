# ifndef PROGRAM_INCLUDE_DELAY_LINE_H_
# define PROGRAM_INCLUDE_DELAY_LINE_H_

# include "netxpto_20180418.h"
# include <vector>

// Adjusts the starting point of the discretized signal
class DelayLine : public Block {

public:

	DelayLine() {};
	DelayLine(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig){};

	void initialize(void);
	bool runBlock(void);

	void setDelay(int Delay) { delay = Delay; delayTime = delay * inputSignals[0]->getSamplingPeriod(); aux = delay; }
	int getDelay() { return delay; }

	void setDelayTime(double dTime) { delayTime = dTime; delay = floor(delayTime / inputSignals[0]->getSamplingPeriod()); aux = delay; }
	double getDelayTime() { return delayTime; }
private:

	bool firstTime{ true };
	bool firstEnd{ true };
	int delay = 0;
	int aux = 0;
	double delayTime = 0.0;

};


#endif // !PROGRAM_INCLUDE_DELAY_LINE_H_

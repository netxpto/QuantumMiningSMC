#ifndef MESSAGE_INTERFACE_H
#define MESSAGE_INTERFACE_H

#include <math.h>

#include "netxpto_20180118.h"

enum message_value_type{ BasisReconciliation };
enum state_machine_type{ EnableToWrite, enableToRead, BitToProcess, IdleState};

class MessageProcessorAlice : public Block {

	/*Input Parameters*/
	int bufferLengthIn{ 512 };
	int bufferLengthOut{ 512 };


	/*State Variables*/
	message_value_type messageType{ BasisReconciliation };
	int messageDataLength{ 64 };

	int* pBufferIn = new int[bufferLengthIn];
	int inPositionBufferIn{ 0 };
	int outPositionBufferIn{ 0 };
	bool bufferEmptyIn{ true };
	bool bufferFullIn{ true };

	int* pBufferOut = new int[bufferLengthOut];
	int inPositionBufferOut{ 0 };
	int outPositionBufferOut{ 0 };
	bool bufferEmptyOut{ true };
	bool bufferFullOut{ true };

	state_machine_type StateMachine;

public:


	/*Methods*/
	MessageProcessorAlice() {};
	MessageProcessorAlice(vector <Signal*> &inputSignals, vector <Signal*> &outputSignals) : Block(inputSignals, outputSignals) {};

	void setBufferLengthIn(int bLength) { bufferLengthIn = bLength; };
	int getBufferLengthIn() { return bufferLengthIn; };

	void setBufferLengthOut(int bLength) { bufferLengthOut = bLength; };
	int getBufferLengthOut() { return bufferLengthOut; };

	int spaceBufferIn(void);
	int spaceBufferOut(void);

	void initialize(void);
	bool runBlock(void);
};


#endif

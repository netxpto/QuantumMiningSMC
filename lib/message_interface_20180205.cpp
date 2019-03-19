
#include "message_interface_20180205.h"



void MessageProcessorAlice::initialize(void) {
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	outputSignals[1]->setSamplingPeriod(inputSignals[1]->getSamplingPeriod());
	outputSignals[1]->setSymbolPeriod(inputSignals[1]->getSymbolPeriod());
	outputSignals[1]->setSamplesPerSymbol(inputSignals[1]->getSamplesPerSymbol());
	outputSignals[1]->setFirstValueToBeSaved(inputSignals[1]->getFirstValueToBeSaved());
}

bool MessageProcessorAlice::runBlock(void) {

	/*Number of message ready to be read*/
	int ready = inputSignals[1]->ready();

	int spaceIn = spaceBufferIn();					//in units of bits
	spaceIn = spaceIn / messageDataLength;		//in units of messages

	int process = min(ready, spaceIn);

	for (auto k = 1; k <= process; k++) {
		t_message message;
		inputSignals[1]->bufferGet(&message);
	}

	/*Number of bits ready to be read*/
	int ready2 = inputSignals[0]->ready();

	/*State variable set*/
	StateMachine = IdleState;

	bool alive = false;

	/*While there are bits to process from binary source*/
/*	while (ready2 != 0) {
		if (ready1 != 0) {
			int mType;
			int emptySpace;

			int space1 = outputSignals[0]->space();
			int space2 = outputSignals[1]->space();

			int length = min(space1, space2);

			if (length <= 0) return false;

			switch (StateMachine) {
				case enableToWrite:
					int auxIn;
					for (int i = 0; i < dataLength; i++) {
						inputSignals[1]->bufferGet(&auxIn);
						pBufferIn[i] = auxIn;
						bIn++;
					}
					newMessage = true;
					StateMachine = idleState;
					break;

				case enableToRead:
					for (bOut = 0; bOut <= bufferLength; bOut++) {
						pBufferOut[bOut] = pBufferIn[bOut];
					}
					StateMachine = bitsToProcess;
					break;

				case bitsToProcess:
					int auxBitsOut;

					while (bIn != 0)
					{
						outputSignals[1]->bufferPut(mType);
						outputSignals[1]->bufferPut(dataLength);

						for (bOut = 0; bOut <= dataLength; bOut++) {
							inputSignals[0]->bufferGet(&auxBitsOut);
							if (pBufferOut[bOut] == 0 || pBufferOut[bOut] == 1) {
								if (pBufferOut[bOut] == auxBitsOut) {
									outputSignals[0]->bufferPut(pBufferOut[bOut]);
									outputSignals[1]->bufferPut(pBufferOut[bOut]);
								}
							}
							else {
								outputSignals[1]->bufferPut(pBufferOut[bOut]);
							}
							bIn--;
						}
					}
					StateMachine = idleState;
					break;

				case idleState:
					emptySpace = bufferLength - bIn;
					if (newMessage == true)
					{
						inputSignals[1]->bufferGet(&mType);
						inputSignals[1]->bufferGet(&dataLength);
						newMessage = false;

						if (mType == basisReconciliation) {
							if (emptySpace >= dataLength) {
								StateMachine = enableToWrite;
							}
							else {
								if (bIn == bufferLength) {
									StateMachine = enableToRead;
								}
							}

						}

					}
					else
						alive = true;

					break;
			}
		}
	}
	
*/	
	return alive;
}

int MessageProcessorAlice::spaceBufferIn() {

	if (bufferFullIn) return 0;
	if (inPositionBufferIn == outPositionBufferIn) return bufferLengthIn;
	if (inPositionBufferIn < outPositionBufferIn) return (outPositionBufferIn - inPositionBufferIn);
	if (outPositionBufferIn >= 0) return (bufferLengthIn - inPositionBufferIn + outPositionBufferIn);
	if (outPositionBufferIn == -1) return (bufferLengthIn - inPositionBufferIn);
	return -1;

};

int MessageProcessorAlice::spaceBufferOut() {

	if (bufferFullOut) return 0;
	if (inPositionBufferOut == outPositionBufferOut) return bufferLengthOut;
	if (inPositionBufferOut < outPositionBufferOut) return (outPositionBufferOut - inPositionBufferOut);
	if (outPositionBufferOut >= 0) return (bufferLengthOut - inPositionBufferOut + outPositionBufferOut);
	if (outPositionBufferOut == -1) return (bufferLengthOut - inPositionBufferOut);
	return -1;

};

#include "message_processor_alice_20180205.h"

void MessageProcessorAlice::initialize(void) {

	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplesPerSymbol(inputSignals[0]->getSamplesPerSymbol());
//	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	outputSignals[1]->setSamplingPeriod(0.001);
	outputSignals[1]->setSymbolPeriod(0.001);
	outputSignals[1]->setSamplesPerSymbol(inputSignals[1]->getSamplesPerSymbol());

}



bool MessageProcessorAlice::runBlock(void) {

	bool process{ false }, alive{ false };

	do {
	//	process = ProcessBasisToStore();
	//	alive = alive || process;
		process = processStoredMessages();
		alive = alive || process;
		process = processInMessages();
		alive = alive || process;
		process = processStoredMessages();
		alive = alive || process;
	} while (process);

	return alive;
}

bool MessageProcessorAlice::ProcessBasisToStore() {
	bool alive{ false };

	int ready = inputSignals[0]->ready();

	int space = maxOfStoredBasis - numberOfStoredBasis;

	int process = min(ready, space);

	if (process > 0) {
		for (auto k = 0; k < process; k++) {
			t_binary basisIn;
			inputSignals[0]->bufferGet(&basisIn);
			storedBasis.push_back((int)basisIn);
			numberOfStoredBasis++;
			alive = true;
		}
	}
	return alive;
}

bool MessageProcessorAlice::processStoredMessages() {

	bool alive{ false };
	int ready = inputSignals[0]->ready();

	int space0 = outputSignals[0]->space();
	int space1 = outputSignals[1]->space();
	int space = min(space0, space1);

	int process = min(space, ready);

	if (process <= 0) return alive;

	for(auto n = 0; n < numberOfStoredMessages; n++) {

		t_message_type mType = getMessageType(storedMessages[n]);
		t_message_data_length mDataLength = getMessageDataLength(storedMessages[n]);
		t_message_data mData = getMessageData(storedMessages[n], mDataLength);


		string mDataOut{ "" };
		int processMessage{ 0 };
		switch (mType) {

			case BasisReconciliation:

				int readyMessage = min(ready, (int)mData.size());
				processMessage = min(readyMessage,space0);

				if (processMessage < 0) return alive;

				for (auto k = 0; k < processMessage; k++) {
					t_binary basisIn;
					inputSignals[0]->bufferGet(&basisIn);
					alive = true;

					if (basisIn == mData[k]) {
						outputSignals[0]->bufferPut((t_binary)1);
						mDataOut.append(to_string((int)1));
					}
					else {
						outputSignals[0]->bufferPut((t_binary)0);
						mDataOut.append(to_string((int)0));
					}
				}
				break;
		}

		int dLength = mDataLength - processMessage;
		mData.erase(mData.begin(), mData.begin() + processMessage);
		if (dLength == 0) {
			storedMessages.erase(storedMessages.begin(),storedMessages.begin() + n+1);
			numberOfStoredMessages = (int)storedMessages.size();
		}
		else {
			storedMessages[n].messageDataLength = to_string((int)mData.size());
			string mDataUpdated{ "" };
			for (unsigned int m = 0; m < mData.size(); m++) {
				mDataUpdated.append(to_string(mData[m]));
			}
			storedMessages[n].messageData = mDataUpdated;
		}

		t_message messageOut;
		messageOut.messageType = mType;
		messageOut.messageDataLength = to_string((int)mDataOut.size());
		messageOut.messageData = mDataOut;

		if (mDataOut.size() != 0)
		{
			outputSignals[1]->bufferPut((t_message)messageOut);
		}
		else;

	}
	
	return alive;
}


bool MessageProcessorAlice::processInMessages() {

	bool alive{ false };

	int ready = inputSignals[1]->ready();
	if (ready > 0) {
		if (numberOfStoredMessages < maxNumberOfStoredMessages) {
			t_message msgIn;
			inputSignals[1]->bufferGet(&msgIn);
			storedMessages.push_back(msgIn);
			numberOfStoredMessages++;
			alive = true;
		}
	}

	return alive;

}


t_message_type MessageProcessorAlice::getMessageType(const t_message& msg) {

	if ((msg.messageType).compare("BasisReconcilitation")==0) return BasisReconciliation;

	return (t_message_type) 0;
}

t_message_data_length MessageProcessorAlice::getMessageDataLength(const t_message& msg) {
	if ((msg.messageDataLength).size() != 0)
		return stoi(msg.messageDataLength);
	else
		return 0;
}


t_message_data MessageProcessorAlice::getMessageData(const t_message& msg, t_message_data_length dataLength) {

	string mDataString = msg.messageData;

	vector <int> mDataVector;

	for (int k = 0; k < dataLength; k++) {
		char data = mDataString.at(k);
		
		if(data == '1')
			mDataVector.push_back(1);
		else if (data == '0')
			mDataVector.push_back(0);
		else if (data == '-') {
			char aux_data = mDataString.at(k + 1);
			if (aux_data == '2') {
				mDataVector.push_back(-2);
				k++;
			}
			else if (aux_data == '1') {
				mDataVector.push_back(-1);
				k++;
			}
		}
	}

	return mDataVector;
}


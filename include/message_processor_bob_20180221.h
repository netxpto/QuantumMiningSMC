#ifndef MESSAGE_PROCESSOR_BOB_H
#define MESSAGE_PROCESSOR_BOB_H

#include <sstream>
#include <iostream>
#include <vector>
#include "bb84_with_discrete_variables_sdf.h"

using namespace std;

class MessageProcessorBob : public Block {
	
public:
	/*Public Methods*/
	MessageProcessorBob() {};
	MessageProcessorBob(vector <Signal*> &inputSignals, vector <Signal*> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);
	bool runBlock(void);

private:
	/*Input Parameters*/
	t_message_data_length maxMessageDataLength{ 64 };
	int maxNumberOfStoredMessages{ 1 };
	int maxOfStoredBasis{ 64 };

	/*State Variables*/
	vector<t_message> storedMessages;
	vector<int> storedBasis;
	int numberOfStoredMessages{ 0 };
	int numberOfStoredBasis{ 0 };

	/*Private Methods*/
	t_message_data_length getMessageDataLength(const t_message& msg);
	void setMessageDataLength(t_message msg, t_message_data_length mDataLength) {msg.messageDataLength = to_string(mDataLength);};
	t_message_type getMessageType(const t_message& msg);
	void setMessageType(t_message msg, t_message_type mType) { msg.messageType = mType; };
	t_message_data getMessageData(const t_message& msg, t_message_data_length dataLength);
	void setMessageData(t_message msg, string mData) { msg.messageData = mData; };

	bool ProcessBasisToStore();
	bool ProcessMessageToSend();
	bool ProcessReceivedMessage();
	bool ProcessStoredMessage();

};
	
#endif // !MESSAGE_PROCESSOR_BOB_H


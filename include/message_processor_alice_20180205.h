#ifndef MESSAGE_INTERFACE_H
#define MESSAGE_INTERFACE_H

#include <math.h>
#include <string.h>

#include "bb84_with_discrete_variables_sdf.h"

class MessageProcessorAlice : public Block {

public:

	/*Public Methods*/
	MessageProcessorAlice() {};
	MessageProcessorAlice(vector <Signal*> &inputSignals, vector <Signal*> &outputSignals) : Block(inputSignals, outputSignals) {};

	void initialize(void);
	bool runBlock(void);

	void setMaxMessageDataLength(t_message_data_length maxDataLength) { maxMessageDataLength = maxDataLength; };
	t_message_data_length getMaxMessageDataLength() { return maxMessageDataLength; };


private:

	/*Input Parameters*/
	t_message_data_length maxMessageDataLength{ 64 };
	int maxNumberOfStoredMessages{ 1 };
	int maxOfStoredBasis{ 64 };

	/*State Variables*/

	vector<t_message> storedMessages;
	int numberOfStoredMessages{ 0 };
	vector<t_binary> storedBasis;
	int numberOfStoredBasis{ 0 };

	/*Private Methods*/
	t_message_type getMessageType(const t_message& msg);
	void setMessageType(t_message msg, t_message_type mType) { msg.messageType = mType; };
	t_message_data_length getMessageDataLength(const t_message& msg);
	void setMessageDataLength(t_message msg, t_message_data_length mDataLength) { msg.messageDataLength = to_string(mDataLength); };
	t_message_data getMessageData(const t_message& msg, t_message_data_length dataLength);
	void setMessageData(t_message msg, string mData) { msg.messageData = mData; };



	bool processStoredMessages();
	bool processInMessages();
	bool ProcessBasisToStore();
};


#endif

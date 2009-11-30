
#ifndef TBX_WIMPMESSAGELISTENER_H_
#define TBX_WIMPMESSAGELISTENER_H_

#include "pollinfo.h"

namespace tbx {

/**
 * Wimp message information.
 */
class WimpMessage
{
public:
	//@{
	//  Use to refer to a message in an existing
	//  wimp poll block.
	//
	// The poll block is NOT deleted when this class is
	// deleted.
	//
	//@param pollBlock Wimp poll block that contains a message
	//@}
	WimpMessage(PollBlock &poll_block);

	//@{
	// Create a new message block for the specified action.
	//
	//@param messageAction the code for the message.
	//@param size size of the message block in (32 bit) words
	//@}
	WimpMessage(int message_id, int size);

	//@{
	// Create a copy of an existing message.
	//
	//@param other message to copy
	//@param size_override - specify the size of the message.
	//       0 means use the other message size.
	//       if greater then the other message size, the extra space
	//       is uninitialised.
	//@}
	WimpMessage(const WimpMessage &other, int size_override = 0);

	//@{
	// Create a reference or copy to another message
	//
	//@param other message to reference/copy
	//@param copy true to create a copy
	//@}
	WimpMessage(const WimpMessage &other, bool copy);

	//@{
	// Assignment creates a copy of an existing message.
	//
	//@param other message to copy.
	//@}
	WimpMessage &operator=(const WimpMessage &other);

	//@{
	// Destructor for WimpMessage. Deletes the message block
	// if it owns it.
	//@}
	virtual ~WimpMessage();

	// Common attributes for all messages
	// length of block, 20 - 256 bytes, a whole number of words
	int block_size() const	    	{return _message_block[0];}
	int sender_task_handle() const 	{return _message_block[1];}
	int my_ref() const				{return _message_block[2];}
	int your_ref() const			{return _message_block[3];}
	int message_id() const			{return _message_block[4];}

	void my_ref(int ref)			{_message_block[2] = ref;}
	void your_ref(int ref)			{_message_block[3] = ref;}
	void message_id(int id) 		{_message_block[4] = id;}

	/**
	 * Return message word
	 */
	int word(int index) const		{return _message_block[index];}

	/**
	 * Return reference to message word
	 */
	int &word(int index)			 {return _message_block[index];}

	/**
	 *  Index a word in the message
	 */
	int operator[] (int index) const {return _message_block[index];}

	/**
	 *  Index a word in the message
	 */
	int &operator[] (int index) {return _message_block[index];}

	/**
	 * Return char * for part of the message
	 */
	const char *str(int index) const {return reinterpret_cast<char *>(_message_block+index);}

	/**
	 * Return char * for part of the message
	 */
	char *str(int index) {return reinterpret_cast<char *>(_message_block+index);}


	// Operations
	enum SendType {User = 17, Recorded = 18, Acknowledge = 19};
	enum SpecialDestination {Broadcast = 0, Iconbar = -2};
	int send(SendType type, int destination, int iconHandle = 0);

protected:
	int *_message_block;
	bool _owns_block;
};


/**
 * Wimp message received event.
 *
 * Call claim to stop further listeners processing the event.
 */
class WimpMessageEvent
{
private:
	WimpMessage _message;
	bool _claimed;

public:
	/**
	 * Constructor
	 */
	WimpMessageEvent(PollBlock &poll_block)  : _message(poll_block), _claimed(false) {}

	/**
	 * WimpMessage that was received.
	 */
	const WimpMessage &message() const  {return _message;}

	/**
	 * Claim the event so no other listeners process it
	 */
	void claim() {_claimed = true;}

	/**
	 * Has the event been claimed.
	 */
	bool claimed() const {return _claimed;}
};

/**
 * Listener for wimp messages
 */
class WimpMessageListener {
public:
	/**
	 * Destructor
	 */
	virtual ~WimpMessageListener() {};

	/**
	 * Override this to process user messages
	 */
	virtual void user_message(WimpMessageEvent &event) {}

	/**
	 * Override this to process recorded user messages.
	 *
	 * These are message that require an acknowledgement.
	 */
	virtual void user_message_recorded(WimpMessageEvent &event, int reply_to) {}

	/**
	 * Override this to process an acknowledgement.
	 */
	virtual void user_message_acknowledge(WimpMessageEvent &event) {}
};

}

#endif /* TBX_WIMPMESSAGELISTENER_H_ */

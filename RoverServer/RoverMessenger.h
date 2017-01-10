#include <boost\lockfree\queue.hpp>
#include <boost\utility\binary.hpp>
#include <boost\atomic.hpp>
#include <boost\thread.hpp>

#include <iostream>

#include "RoverCommands.h"
#include "RoverExecuter.h"
#include "IRoverExecuterDelegate.h"

#ifndef __ROVERMESSENGER__
#define __ROVERMESSENGER__

using namespace boost;

class RoverMessenger {
public:
	RoverMessenger(IRoverExecuterDelegate * roverExecuterDelegate);
	~RoverMessenger();
	void AddMessage(const ROVERMESSAGE &msg);
	void StartQueueProcessing();
	void StopQueueProcessing();
	bool isQueueRunning();
protected:
	void _messageQueueProcess();
	void _processMessage(const ROVERMESSAGE &msg);
private:
	lockfree::queue<ROVERMESSAGE> __lfq;
	atomic<bool> __queueRunning;
	atomic<bool> __stopQueueRequest;
	thread __threadProcess;
	RoverExecuter __re;
};


#endif
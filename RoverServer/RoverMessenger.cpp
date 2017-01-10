#include "RoverMessenger.h"

RoverMessenger::RoverMessenger(IRoverExecuterDelegate * roverExecuterDelegate) : __queueRunning(false), __stopQueueRequest(false), __lfq(128), __re(roverExecuterDelegate) {

}

RoverMessenger::~RoverMessenger() {
	if (isQueueRunning()) StopQueueProcessing();
}

void RoverMessenger::AddMessage(const ROVERMESSAGE &msg) {
	__lfq.push(msg);
}

void RoverMessenger::StartQueueProcessing() {
	if (isQueueRunning()) return;
	__threadProcess = thread(&RoverMessenger::_messageQueueProcess,this);
}

void RoverMessenger::StopQueueProcessing() {
	if (!isQueueRunning()) return;
	__stopQueueRequest = true;
	__threadProcess.join();
	__stopQueueRequest = false;
	__queueRunning = false;
}

void RoverMessenger::_processMessage(const ROVERMESSAGE &msg) {
	// Process Command Here
	__re.ProcessRoverMessage(msg);
}

void RoverMessenger::_messageQueueProcess() {
	__queueRunning = true;

	ROVERMESSAGE msg;

	while (!__stopQueueRequest) {
		if (__lfq.pop(msg)) {
			// Have a message in Queue
			_processMessage(msg);
		}
	}
	// Cleanup Queue Here

	__queueRunning = false;
}

bool RoverMessenger::isQueueRunning() {
	return __queueRunning;
}
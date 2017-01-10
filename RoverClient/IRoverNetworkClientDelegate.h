#include "ILog.h"
#include "../RoverServer/RoverCommands.h"

#ifndef __IROVERNETWORKCLIENTDELEGATE__
#define __IROVERNETWORKCLIENTDELEGATE__

class IRoverNetworkClientDelegate : public ILog {
public:
	virtual void onClientConnect() = 0;
	virtual void onClientDisconnect() = 0;
	virtual void onFailedToConnect() = 0;
	virtual void onClientConnecting() = 0;
	virtual void onMessageRecieve(const ROVERMESSAGE &rm) = 0;
};

#endif
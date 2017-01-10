#include <Windows.h>
#include <string>

#include "../RoverServer/RoverCommands.h"
#include "ILog.h"

#ifndef __IAPPDELEGATE__
#define __IAPPDELEGATE__

class IAppDelegate : public ILog {
public:
	virtual void onMainWindowClose(void) = 0;
	virtual void requestConnection(const std::string &ipAddress, int port) = 0;
	virtual void requestDisconnect() = 0;
	virtual void commandRequest(const ROVERMESSAGE &rm) = 0;
};

#endif
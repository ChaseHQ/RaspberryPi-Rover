#include "ILog.h"

#ifndef __IROVERNETWORKCLIENTDELEGATE__
#define __IROVERNETWORKCLIENTDELEGATE__

class IRoverNetworkClientDelegate : public ILog {
public:
	virtual void onClientConnect() = 0;
	virtual void onClientDisconnect() = 0;
};

#endif
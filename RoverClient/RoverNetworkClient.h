#include "IRoverNetworkClientDelegate.h"

#include <string>

#ifndef __ROVERNETWORKCLIENT__
#define __ROVERNETWORKCLIENT__

class RoverNetworkClient {
public:
	RoverNetworkClient(IRoverNetworkClientDelegate &RNCDelegate);
	~RoverNetworkClient();
	void ConnectToClient(const std::string &ipAddress, int port);
protected:
private:
	IRoverNetworkClientDelegate * __rncDelegate;
};

#endif
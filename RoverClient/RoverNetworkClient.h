#include "IRoverNetworkClientDelegate.h"


#ifndef __ROVERNETWORKCLIENT__
#define __ROVERNETWORKCLIENT__

class RoverNetworkClient {
public:
	RoverNetworkClient(IRoverNetworkClientDelegate * RNCDelegate);
	~RoverNetworkClient();
protected:
private:
	IRoverNetworkClientDelegate * __rncDelegate;
};

#endif
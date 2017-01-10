#include "RoverCommands.h"

#ifndef __IROVEREXECUTERDELEGATE__
#define __IROVEREXECUTERDELEGATE__

class IRoverExecuterDelegate {
public:
	virtual void sendResponseMessage(const ROVERMESSAGE& rm) = 0;
};

#endif
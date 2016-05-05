#ifndef __ILOGGER__
#define __ILOGGER__

class ILogger {
public:
	virtual void log(const char * LogMsg) = 0;
};

#endif
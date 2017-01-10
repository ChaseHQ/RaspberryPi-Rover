#ifdef _WIN32
#include "Win32WiringPi.h"
#else
#include <wiringPi.h>
#include <wiringPiSPI.h>
#endif

#include <boost/utility/binary.hpp>

#include "RoverCommands.h"
#include "IRoverExecuterDelegate.h"

#define uchar unsigned char

#ifndef __ROVEREXECUTER__
#define __ROVEREXECUTER__

class RoverExecuter {
public:
	RoverExecuter(IRoverExecuterDelegate * roverExecuterDelegate);
	~RoverExecuter();
	void InitializeRover(void);
	void RoverStop(void);
	void RoverBack(void);
	void RoverLeft(void);
	void RoverRight(void);
	void RoverForward(void);
	void RoverHorn(void);
	void SetTrimSpeed(unsigned int leftTrim, unsigned int rightTrim);
	float GetDistance(void);
	void ProcessRoverMessage(const ROVERMESSAGE &msg);
protected:
	void _SendTrimSpeed();
private:
	uchar __currentMovement;
	uchar __requestedMovement;
	unsigned int __leftSpeedTrim;
	unsigned int __rightSpeedTrim;
	IRoverExecuterDelegate * __roverExecuterDelegate;
};

#endif
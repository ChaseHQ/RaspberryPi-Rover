#include "RoverExecuter.h"

RoverExecuter::RoverExecuter() : __leftSpeedTrim(103), __rightSpeedTrim(127), __currentMovement(ROVER_STOP), __requestedMovement(ROVER_STOP) {
  wiringPiSetup();
  wiringPiSPISetup(0,1000000);

  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(6, OUTPUT);

  InitializeRover();
}

RoverExecuter::~RoverExecuter() {
	RoverStop();
}

void RoverExecuter::InitializeRover() {
	RoverStop();
}

void RoverExecuter::RoverHorn() {
	digitalWrite(6,1);
}

void RoverExecuter::ProcessRoverMessage(const ROVERMESSAGE &msg) {
	if (msg.ver != ROVERMESSAGE_VERSION) {
		std::cout << "RoverExecuter::ProcessRoverMessage - Processed Message Is Incorrect Version! Expected " << ROVERMESSAGE_VERSION << " Got " << msg.ver << std::endl;
		return;
	}

	switch (msg.cmd) {
	case ROVER_STOP:
		RoverStop();
		break;
	case ROVER_LEFT:
		RoverLeft();
		break;
	case ROVER_RIGHT:
		RoverRight();
		break;
	case ROVER_FORWARD:
		RoverForward();
		break;
	case ROVER_BACK:
		RoverBack();
		break;
	case ROVER_HORN:
		RoverHorn();
		break;
	case ROVER_SETTRIM:
		SetTrimSpeed(msg.data.sWord.high,msg.data.sWord.low);
		break;
	}
}

void RoverExecuter::RoverStop() {
	digitalWrite(0,0);
	digitalWrite(1,0);
	digitalWrite(2,0);
	digitalWrite(3,0);
	digitalWrite(6,0);

	uchar lCommand    = ROVER_INTERNAL_COMMAND_SETLWHEEL;
	uchar rCommand    = ROVER_INTERNAL_COMMAND_SETRWHEEL;
	uchar wheelLSpeed = BOOST_BINARY (00000000);
	uchar wheelRSpeed = BOOST_BINARY (00000000);

	wiringPiSPIDataRW(0,&lCommand,1);
	wiringPiSPIDataRW(0,&wheelLSpeed,1);
	wiringPiSPIDataRW(0,&rCommand,1);
	wiringPiSPIDataRW(0,&wheelRSpeed,1);
}

void RoverExecuter::RoverBack() {
	digitalWrite(0,0);
	digitalWrite(1,1);
	digitalWrite(2,0);
	digitalWrite(3,1);

	uchar lCommand    = ROVER_INTERNAL_COMMAND_SETLWHEEL;
	uchar rCommand    = ROVER_INTERNAL_COMMAND_SETRWHEEL;
	uchar wheelLSpeed = BOOST_BINARY (01111111);
	uchar wheelRSpeed = BOOST_BINARY (01111111);

	wiringPiSPIDataRW(0,&lCommand,1);
	wiringPiSPIDataRW(0,&wheelLSpeed,1);
	wiringPiSPIDataRW(0,&rCommand,1);
	wiringPiSPIDataRW(0,&wheelRSpeed,1);
}

void RoverExecuter::RoverLeft() {
	digitalWrite(0,0);
	digitalWrite(1,0);
	digitalWrite(2,1);
	digitalWrite(3,0);

	uchar lCommand    = ROVER_INTERNAL_COMMAND_SETLWHEEL;
	uchar rCommand    = ROVER_INTERNAL_COMMAND_SETRWHEEL;
	uchar wheelLSpeed = BOOST_BINARY (01111111);
	uchar wheelRSpeed = BOOST_BINARY (01111111);

	wiringPiSPIDataRW(0,&lCommand,1);
	wiringPiSPIDataRW(0,&wheelLSpeed,1);
	wiringPiSPIDataRW(0,&rCommand,1);
	wiringPiSPIDataRW(0,&wheelRSpeed,1);
}

void RoverExecuter::RoverRight() {
	digitalWrite(0,1);
	digitalWrite(1,0);
	digitalWrite(2,0);
	digitalWrite(3,0);

	uchar lCommand    = ROVER_INTERNAL_COMMAND_SETLWHEEL;
	uchar rCommand    = ROVER_INTERNAL_COMMAND_SETRWHEEL;
	uchar wheelLSpeed = BOOST_BINARY (01111111);
	uchar wheelRSpeed = BOOST_BINARY (01111111);

	wiringPiSPIDataRW(0,&lCommand,1);
	wiringPiSPIDataRW(0,&wheelLSpeed,1);
	wiringPiSPIDataRW(0,&rCommand,1);
	wiringPiSPIDataRW(0,&wheelRSpeed,1);
}

void RoverExecuter::RoverForward() {
	digitalWrite(0,1);
	digitalWrite(1,0);
	digitalWrite(2,1);
	digitalWrite(3,0);

	uchar lCommand    = ROVER_INTERNAL_COMMAND_SETLWHEEL;
	uchar rCommand    = ROVER_INTERNAL_COMMAND_SETRWHEEL;
	uchar wheelLSpeed = __leftSpeedTrim;
	uchar wheelRSpeed = __rightSpeedTrim;

	wiringPiSPIDataRW(0,&lCommand,1);
	wiringPiSPIDataRW(0,&wheelLSpeed,1);
	wiringPiSPIDataRW(0,&rCommand,1);
	wiringPiSPIDataRW(0,&wheelRSpeed,1);
}

void RoverExecuter::SetTrimSpeed(unsigned int leftTrim, unsigned int rightTrim) {
	if (leftTrim > 127) leftTrim = 127;
	if (rightTrim > 127) rightTrim = 127;
	__leftSpeedTrim = leftTrim;
	__rightSpeedTrim = rightTrim;
}

float RoverExecuter::GetDistance() {
	std::cout << "RoverExecuter::GetDistance() - Executed" << std::endl;
	unsigned char commandBuffer = ROVER_INTERNAL_COMMAND_GETDISTANCE;
	wiringPiSPIDataRW(0,&commandBuffer,1);
	while (commandBuffer != ROVER_INTERNAL_COMMAND_QUERY) {
		commandBuffer = ROVER_INTERNAL_COMMAND_NOCOMMAND;
		wiringPiSPIDataRW(0,&commandBuffer,1);
		if (commandBuffer == ROVER_INTERNAL_COMMAND_BADSTATE || commandBuffer == ROVER_INTERNAL_COMMAND_TOOMANYNOC) {
			std::cout << "RoverExecuter::GetDistance() - Previous Distance Get Running - Bad Command" << std::endl;
			return -1.0f;
		}
	}
	while (commandBuffer != ROVER_INTERNAL_COMMAND_DISTREADY) {
		commandBuffer = ROVER_INTERNAL_COMMAND_QDISTREADY;
		wiringPiSPIDataRW(0,&commandBuffer,1);
	}

	word testDistance;
	testDistance.val = 0;
	commandBuffer = ROVER_INTERNAL_COMMAND_RETDISTANCEH;
	wiringPiSPIDataRW(0,&commandBuffer,1);
	commandBuffer = ROVER_INTERNAL_COMMAND_RETDISTANCEL;
	wiringPiSPIDataRW(0,&commandBuffer,1);
	testDistance.sWord.high = commandBuffer;
	commandBuffer = ROVER_INTERNAL_COMMAND_NOCOMMAND;
	wiringPiSPIDataRW(0,&commandBuffer,1);
	testDistance.sWord.low = commandBuffer;

	float distance = ((float)testDistance.val / 1000.0f); 
	distance = (340.0f * distance) / 2.0f; // Convert to mm
	distance = (distance * 0.0393701f); // Convert to Inches

	return distance;
}

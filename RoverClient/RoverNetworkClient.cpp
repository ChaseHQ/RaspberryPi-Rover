#include "RoverNetworkClient.h"

RoverNetworkClient::RoverNetworkClient(IRoverNetworkClientDelegate &RNCDelegate) : __rncDelegate(&RNCDelegate) {
	
}

RoverNetworkClient::~RoverNetworkClient() {

}

void RoverNetworkClient::ConnectToClient(const std::string &ipAddress, int port) {
	__rncDelegate->log((std::string("RoverNetworkClient::ConnectToClient - Connection request to - ") + ipAddress + " - on port - " + std::to_string(port)).c_str());
}
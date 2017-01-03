#include "RoverNetworkClient.h"

RoverNetworkClient::RoverNetworkClient(IRoverNetworkClientDelegate &RNCDelegate) : __rncDelegate(&RNCDelegate), __connecting(false), __connected(false) {
	
}

RoverNetworkClient::~RoverNetworkClient() {

}

void RoverNetworkClient::ConnectToClient(const std::string &ipAddress, int port) {
	if (__connecting || __connected) return;

	__rncDelegate->log((std::string("RoverNetworkClient::ConnectToClient - Connection request to - ") + ipAddress.c_str() + " - on port - " + std::to_string(port)).c_str());

	_aios.reset();

	_clientSocket.reset(new asio::ip::tcp::socket(_aios));

	_clientSocket->async_connect(asio::ip::tcp::endpoint(asio::ip::address_v4::from_string(ipAddress), port), bind(&RoverNetworkClient::_connectHandler, this, asio::placeholders::error));
	__connecting = true;
	__rncDelegate->onClientConnecting();
	_threadIOService = thread(bind(&asio::io_service::run, &_aios));
}

void RoverNetworkClient::_connectHandler(const system::error_code& er) {
	__connecting = false;
	if (!er) {
		__rncDelegate->log("RoverNetworkClient::_connectHandler - Connected to server!");
		__connected = true;
		_threadRecieveThread = thread(bind(&RoverNetworkClient::_recieveThread, this));
		__rncDelegate->onClientConnect();
	} else {
		__rncDelegate->log("RoverNetworkClient::_connectHandler - Failed to connect to address");
		__rncDelegate->onFailedToConnect();
	}
}

void RoverNetworkClient::_recieveThread() {
	__rncDelegate->log("RoverNetworkClient::_recieveThread() - Started...");
	system::error_code error;
	while (error != asio::error::eof && error != asio::error::operation_aborted && error != asio::error::connection_aborted && error != asio::error::connection_reset) {
		boost::array<unsigned char, sizeof(ROVERMESSAGE)> buffer;
		size_t lenRec = read(*_clientSocket, asio::buffer(buffer, sizeof(ROVERMESSAGE)), error);
		if (lenRec == sizeof(ROVERMESSAGE)) {
			// size matches - legit message
			ROVERMESSAGE rm((ROVERMESSAGE *)buffer.data());
			// Do something with the message
		}
	}
	__connected = false;
	__rncDelegate->log("RoverNetworkClient::_recieveThread() - Stopped...");
	__rncDelegate->log("RoverNetworkClient::_recieveThread() - Issuing Disconnect Messages");
	__rncDelegate->onClientDisconnect();
}

void RoverNetworkClient::Disconnect() {
	if (!__connected) return;
	__rncDelegate->log("RoverNetworkClient::Disconnect() - Disconnect Request Issued");
	_clientSocket->close();
}

bool RoverNetworkClient::isConnected() {
	return __connected;
}
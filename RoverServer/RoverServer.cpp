#include "RoverServer.h"

CRoverServer::CRoverServer() : __serverRunning(false), __serverListening(false), __recieveThreadRunning(false), __shuttingDown(false), __shutdownRequest(false) {
	
}

CRoverServer::~CRoverServer(void) {
	
}

void CRoverServer::startServer(int port) {
	if (__serverRunning || __serverListening) return;
	if (port < 1 || port > 65535) return;

	_aios.reset();

	_serverAcceptor.reset( new asio::ip::tcp::acceptor(_aios,asio::ip::tcp::endpoint(asio::ip::tcp::v4(),port)) );
	_serverSocket.reset( new asio::ip::tcp::socket(_aios) );

	__serverRunning = true;
	__serverListening = true;

	_serverAcceptor->async_accept(*_serverSocket, bind(&CRoverServer::_acceptConnection, this, asio::placeholders::error));
	_threadIOService = thread(bind(&asio::io_service::run,&_aios));

	std::cout << "Server Listening on Port " << port << std::endl;
}

bool CRoverServer::isServerRunning() {
	return __serverRunning;
}

void CRoverServer::serverTick() {
	if (__serverRunning && __shutdownRequest)
		_startShutdown();
}

void CRoverServer::shutdownServer() {
	if (!__shuttingDown && !__shutdownRequest)
		__shutdownRequest = true;
}

void CRoverServer::_startShutdown() {
	__shuttingDown = true;

	std::cout << "Server Shutting Down..." << std::endl;
	
	if (__serverListening) {
		_serverAcceptor->cancel();
		_serverAcceptor->close();
		__serverListening = false;
	}

	if (__recieveThreadRunning) {
		_serverSocket->close();
		_threadRecieve.join();
	}

	_aios.stop();
	_threadIOService.join();

	_serverSocket.reset();
	_serverAcceptor.reset();

	__serverRunning = false;

	__shuttingDown = false;
	__shutdownRequest = false;
}

void CRoverServer::_recieveThread() {
	std::cout << "Recieve thread started..." << std::endl;
	__recieveThreadRunning = true;
	__rmsg.StartQueueProcessing();
	system::error_code error;
	while (error != asio::error::eof && error != asio::error::operation_aborted && error != asio::error::connection_aborted) {
		boost::array<unsigned char, sizeof(ROVERMESSAGE)> buffer;
		size_t lenRec = read(*_serverSocket,asio::buffer(buffer,sizeof(ROVERMESSAGE)),error);
		if (lenRec == sizeof(ROVERMESSAGE)) {
			// size matches - legit message
			ROVERMESSAGE rm((ROVERMESSAGE *)buffer.data());
			__rmsg.AddMessage(rm);
		}
	}
	__shutdownRequest = true;
	__rmsg.StopQueueProcessing();
}

void CRoverServer::_acceptConnection(const system::error_code& ec) {
	if (ec == asio::error::operation_aborted) {
		std::cout << "Listening Operation Aborted..." << std::endl;
		return;
	}
	if (ec) {
		std::cout << "Unknown Error while listening..." << std::endl;
		shutdownServer();
		return;
	}
	std::cout << "Recieved Connection from: " << _serverSocket->remote_endpoint().address() << std::endl;
	__serverListening = false;
	_threadRecieve = thread(&CRoverServer::_recieveThread,this); // Start Recieve Thread
}
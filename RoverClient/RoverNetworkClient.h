#ifdef _WIN32
#define _WIN32_WINNT 0x0601
#endif

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/atomic.hpp>
#include <cstddef>

#include "IRoverNetworkClientDelegate.h"
#include "../RoverServer/RoverCommands.h"

#include <string>

#ifndef __ROVERNETWORKCLIENT__
#define __ROVERNETWORKCLIENT__

using namespace boost;

class RoverNetworkClient {
public:
	RoverNetworkClient(IRoverNetworkClientDelegate &RNCDelegate);
	~RoverNetworkClient();
	void ConnectToClient(const std::string &ipAddress, int port);
	void Disconnect();
	bool isConnected();
	void sendRoverCommand(const ROVERMESSAGE& rm);
protected:
	asio::io_service _aios;
	shared_ptr<asio::ip::tcp::socket> _clientSocket;
	shared_ptr<asio::ip::tcp::acceptor> _clientAcceptor;
	IRoverNetworkClientDelegate * __rncDelegate;
	void _connectHandler(const system::error_code& er);
	void _recieveThread();
	void _dataWriteHandler(const system::error_code& ec, std::size_t bytes_sent);
private:
	thread _threadIOService;
	thread _threadRecieveThread;
	atomic<bool> __connecting;
	atomic<bool> __connected;
};

#endif
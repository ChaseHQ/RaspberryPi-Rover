#ifdef _WIN32
#define _WIN32_WINNT 0x0601
#endif

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/atomic.hpp>

#include <iostream>

#include "RoverMessenger.h"
#include "IRoverExecuterDelegate.h"

#ifndef __ROVERSERVER__
#define __ROVERSERVER__

using namespace boost;

class CRoverServer : public IRoverExecuterDelegate
{
public:
	CRoverServer();
	~CRoverServer(void);
	void startServer(int port);
	bool isServerRunning();
	void shutdownServer();
	void serverTick();
	void sendResponseMessage(const ROVERMESSAGE& rm);
protected:
	void _recieveThread();
	void _startShutdown();
	void _acceptConnection(const system::error_code& ec);
	void _dataWriteHandler(const system::error_code& ec, std::size_t bytes_sent);
	thread _threadRecieve;
	thread _threadIOService;
	shared_ptr<asio::ip::tcp::socket> _serverSocket;
	shared_ptr<asio::ip::tcp::acceptor> _serverAcceptor;
	asio::io_service _aios;
private:
	atomic<bool> __shuttingDown;
	atomic<bool> __serverRunning;
	atomic<bool> __serverListening;
	atomic<bool> __recieveThreadRunning;
	atomic<bool> __shutdownRequest;
	RoverMessenger __rmsg;
};

#endif
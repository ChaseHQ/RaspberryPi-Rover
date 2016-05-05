/*
	Rover Server Entry Point
	Craig Vella @ 2016
*/

#include <boost/program_options.hpp>
#include <iostream>
#include "RoverServer.h"

CRoverServer rs;

bool shutdownServer = false;

void intHandler(int interrupt) {
	shutdownServer = true;
	rs.shutdownServer();
}

int main(int argc, char ** argv) {
	
	boost::program_options::options_description poDesc;
	poDesc.add_options()
		("help","Show Help Message")
		("port",boost::program_options::value<int>(),"The Server's Port to run on (1 - 65535)")
		;
	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc,argv,poDesc),vm);
	boost::program_options::notify(vm);

	if (vm.count("help")) {
		std::cout << poDesc << std::endl;
		return 1;
	}

	if (vm.count("port")) {
		if (vm["port"].as<int>() < 1 || vm["port"].as<int>() > 65535) {
			std::cout << "ERROR: Port Number Must be between 1 and 65535" << std::endl;
			return 3;
		} 
	} else {
		std::cout << "ERROR: You must supply a Server Port Number" << std::endl;
		std::cout << poDesc << std::endl;
		return 2;
	}

	signal(SIGINT,intHandler);
	
	
	while (!shutdownServer) {
		rs.startServer(vm["port"].as<int>());
		while (rs.isServerRunning()) {
			rs.serverTick();
			Sleep(1);
		}
	}

	std::cout << "Server Shutdown - Exiting" << std::endl;

	return 0;
}
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <heartbeat.h>
#include <global_config.h>

using namespace Gamecontroller;

int main (int argc, char ** argv) {
	// You can start the heartbeat at any time. Even before having set all the mandatory parameters.
	Heartbeat::startHeartbeat();


	// At some later point you can set all 3 required parameters. The heartbeat will start sending as soon as all 3 are set.
	GlobalConfig::jerseynumber = 2;
	GlobalConfig::teamnumber = 3;
	struct sockaddr_in destination;
	memset(&destination, 0, sizeof(destination)); // You should initialize this with the values you get from recvfrom()
	Heartbeat::set_destination_once(&destination);

	return EXIT_SUCCESS;
}


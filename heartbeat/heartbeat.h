#ifndef HEARTBEAT_H
#define HEARTBEAT_H

/**
 * @file heartbeat.h
 * @brief See class Heartbeat for documentation.
 */

#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <RoboCupGameControlData.h>

namespace Gamecontroller {

/**
 * @brief The heartbeat which signals the gamecontroller that the robot is still alive.
 * Sends the heartbeat signal once a second via a dedicated thread. All public methods are
 * thread safe. Will begin sending the heartbeat signal as soon as jersey number, team
 * number and destination address were set at least once.
 */
class Heartbeat {
		static pthread_mutex_t send_mutex;
		static pthread_t sender_thread;
		static struct sockaddr_in * destination;
		static void * thread_main(void * arg);
		static int sock;
		static struct RoboCupGameControlReturnData * message;
		static struct RoboCupGameControlReturnData * init_message();

		Heartbeat();
		Heartbeat(Heartbeat & h);
		void operator=(Heartbeat const & h);
	public:
		static void startHeartbeat();
		static void set_destination(struct sockaddr_in * d);
		static void set_destination_once(struct sockaddr_in * d);
};

}
#endif /* HEARTBEAT_H */

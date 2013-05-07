
/**
 * @file heartbeat.cpp
 * @brief See class Heartbeat for documentation.
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>


#include <pthread.h>

#include <heartbeat.h>
#include <pthread_helpers.h>
#include <global_config.h>

namespace Gamecontroller {

pthread_mutex_t Heartbeat::send_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t Heartbeat::sender_thread;

int Heartbeat::sock = 0;
struct sockaddr_in* Heartbeat::destination = NULL;
struct RoboCupGameControlReturnData* Heartbeat::message = NULL;

struct RoboCupGameControlReturnData* Heartbeat::init_message() {
	struct RoboCupGameControlReturnData* result = (struct RoboCupGameControlReturnData*) calloc(1, sizeof(*message));
	if (result == NULL) {
		fprintf(stderr, "Error: calloc() failed in file %s line %d\n", __FILE__, __LINE__);
		exit(1);
	}

	memcpy(result->header, GAMECONTROLLER_RETURN_STRUCT_HEADER, sizeof(char) * 4);
	result->version = GAMECONTROLLER_RETURN_STRUCT_VERSION;
	result->team = NO_TEAM_NUMBER;
	result->player = NO_JERSEY_NUMBER;
	result->message = GAMECONTROLLER_RETURN_MSG_ALIVE;

	return result;
}

void Heartbeat::startHeartbeat() {
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		fprintf(stderr, "Error: socket() failed in file %s line %d\n", __FILE__, __LINE__);
		exit(1);
	}

    const int ON = true;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &ON, sizeof(ON));

    pthread_start_detach("GC:Heartbeat", sender_thread, Heartbeat::thread_main, NULL);
}

void * Heartbeat::thread_main(void * arg) {
	const int NO_FLAGS = 0;
    message = init_message();

	while (true) {
		sleep(1);
		if (destination == NULL || message->team == NO_TEAM_NUMBER || message->player == NO_JERSEY_NUMBER) continue;
		
		pthread_mutex_lock(&send_mutex);
		const ssize_t message_len = sizeof(* message);
		message->team = GlobalConfig::teamnumber;
		message->player = GlobalConfig::jerseynumber;
		ssize_t rc = sendto(sock, message, message_len, NO_FLAGS, (struct sockaddr *) destination, sizeof(* destination));
		if (message_len != rc) {
			fprintf(stderr, "Error: sendto() failed in file %s line %d\n", __FILE__, __LINE__);
		}
		pthread_mutex_unlock(&send_mutex);
	}

	return NULL;
}

/**
 * @brief Set the destination address once. Nothing will be done if the destination address was set already. Note that the destination port will be overwritten by this method with the proper gamecontroller port.
 * @param d The address of the gamecontroller. Will not be changed or freed by this method.
 */
void Heartbeat::set_destination_once(struct sockaddr_in * d) {
	if (destination == NULL) {
		set_destination(d);
	}
}

/**
 * @brief Set the destination address. Note that the destination port will be overwritten by this method with the proper gamecontroller port.
 * @param d The address of the gamecontroller. Will not be changed or freed by this method.
 */
void Heartbeat::set_destination(struct sockaddr_in * d) {
	pthread_mutex_lock(&send_mutex);
	if (destination == NULL) {
		destination = (struct sockaddr_in *) malloc(sizeof(* destination));
		if (destination == NULL) {
			fprintf(stderr, "Error: malloc() failed in file %s line %d\n", __FILE__, __LINE__);
			exit(1);
		}
	}

	memcpy(destination, d, sizeof(* destination));
	destination->sin_port = htons(GAMECONTROLLER_PORT);
	pthread_mutex_unlock(&send_mutex);
}

}

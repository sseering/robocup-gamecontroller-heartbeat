#ifndef GLOBAL_CONFIG_H
#define GLOBAL_CONFIG_H

#include <string>
#include <cstdint>

typedef uint16_t jerseynumber_t; // Type is given by RoboCupGameControlData.h which is given to us by the SPL organizers. Valid Range is 1..WM_NUM_PLAYERS inclusive.
typedef uint16_t teamnumber_t; // Type is given by RoboCupGameControlData.h which is given to us by the SPL organizers.

#define NO_JERSEY_NUMBER ((jerseynumber_t)-1)
#define NO_TEAM_NUMBER ((teamnumber_t)-1)

class GlobalConfig {
	public:
		volatile static jerseynumber_t jerseynumber; //<! 1.. WM_NUM_PLAYERS inclusive
		volatile static teamnumber_t teamnumber;
};

#endif

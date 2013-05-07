CXXFLAGS?=-Wall -Wextra -pedantic -Wno-unused-parameter -std=c++11
BINARIES= global_config/global_config.o heartbeat/heartbeat.o replace-this-with-your-own-implementation

all: $(BINARIES)

global_config/global_config.o: global_config/global_config.cpp global_config/global_config.h
	$(CXX) $(CXXFLAGS) -I global_config -fPIC -shared -o $@ $<

heartbeat/heartbeat.o: heartbeat/heartbeat.cpp heartbeat/heartbeat.h global_config/global_config.h include/pthread_helpers.h gamecontroller-implementation/RoboCupGameControlData.h
	$(CXX) $(CXXFLAGS) -I heartbeat -I gamecontroller-implementation -I include -I global_config -fPIC -shared -o $@ $<

replace-this-with-your-own-implementation: replace-this-with-your-own-implementation.cpp heartbeat/heartbeat.o global_config/global_config.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -I global_config -I heartbeat -I gamecontroller-implementation -lpthread -o $@ $^

clean:
	$(RM) *.o *.a *.so $(BINARIES)


#include "interface/interface.h"
#include "network/network.h"


namespace Main {
    typedef struct {
        std::string Name;
        std::string Path;
    } Texture;

    extern bool Running;

    extern std::string Path;
    extern Interface Window;
    extern bool needs_window;
    extern int tick_time_ms;

    extern Server server;
    extern Client client;
    extern Network::NetworkState NetworkingConf;
}
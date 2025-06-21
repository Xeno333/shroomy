#include "interface/interface.h"


namespace Main {
    typedef struct {
        std::string Name;
        std::string Path;
    } Texture;

    extern std::string Path;
    extern Interface Window;
    extern bool Running;
}
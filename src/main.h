#include "interface/interface.h"


namespace Main {
    typedef struct {
        std::string Name;
        std::string Path;
    } Texture;

    extern Interface Window;
    extern bool Running;
}
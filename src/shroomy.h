#ifndef SHROOMY
    #include <iostream>

    #ifdef __GNUC__
        #define DIR_SEP_CHAR "/"
    #endif

    #define SHROOMY_TICK_RATE_MS 100

    namespace Shroomy {
        static const std::string Version = "Shroomy v0.1";
    }
    #define SHROOMY true
#endif
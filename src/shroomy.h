#ifndef SHROOMY
    #include <iostream>

    #ifdef __GNUC__
        #define DIR_SEP_CHAR "/"
    #endif

    namespace Shroomy {
        #ifdef SHROOMY_DEBUG
            static const std::string Version = "[DEBUG] Shroomy v0.1";
        #else
            static const std::string Version = "Shroomy v0.1";
        #endif
    }
    #define SHROOMY true
#endif
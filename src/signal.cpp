#include <csignal>
#include <iostream>
#include "main.h"
#include "shroomysignal.h"

void HandelSystemQuitSignals(int signal) {
    std::cout << signal << std::endl;
    if (signal == SIGTERM)
        abort();
    else if (signal == SIGQUIT || signal == SIGINT)
        Main::Running = false;
}


void ShroomySignals::RegisterSignalHandelers() {
    std::signal(SIGQUIT, HandelSystemQuitSignals);
    std::signal(SIGTERM, HandelSystemQuitSignals);
    std::signal(SIGINT, HandelSystemQuitSignals);
}
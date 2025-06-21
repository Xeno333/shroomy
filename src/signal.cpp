#include <csignal>
#include <iostream>
#include "main.h"
#include "signal.h"

void HandelSystemQuitSignals(int signal) {
    std::cout << signal << std::endl;
    if (signal == SIGQUIT || signal == SIGTERM || signal == SIGINT)
        Main::Running = false;
}


void Signals::RegisterSignalHandelers() {
    std::signal(SIGQUIT, HandelSystemQuitSignals);
    std::signal(SIGTERM, HandelSystemQuitSignals);
    std::signal(SIGINT, HandelSystemQuitSignals);
}
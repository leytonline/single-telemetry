#include <stdio.h>
#include <mutex>
#include <string>
#include <ctime>
#include <sstream>

#ifndef SINGLETELEMTRY_H
#define SINGLETELEMTRY_H

namespace SingleTelemetry {
    int SetFile(std::string);
    void Write(std::string);
    int Close();

    template <typename F, typename... A>
    void TimeFunction(std::string, F fn, A&&... args);
}

#endif

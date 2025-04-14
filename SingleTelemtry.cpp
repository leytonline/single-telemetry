#include "SingleTelemetry.h"

static FILE* _output = nullptr;
static std::mutex _mutex;

int SingleTelemetry::SetFile(std::string str) {
    if (_output != NULL)
    {
        return -EEXIST;
    }

    _output = fopen(str.c_str(), "w");

    return EXIT_SUCCESS;
}

void SingleTelemetry::Write(std::string str) {
    std::lock_guard<std::mutex> lock(_mutex); // says fprintf is thread safe but just for exercise
    fprintf(_output, "%s", str.c_str());
}

int SingleTelemetry::Close() {
    if (!_output) return -ENOENT;
    fclose(_output);
    _output = nullptr;
    return EXIT_SUCCESS;
}

template <typename F, typename... A>
void SingleTelemetry::TimeFunction(std::string str, F fn, A&&... args) {

    clock_t start = clock();

    fn(std::forward<A>(args)...);

    clock_t end = clock();

    std::stringstream ss;

    if (str.length() != 0)
    {
        ss << str << ": ";
    }

    ss << (end - start) << '\n';

    Write(ss.str().c_str());
}

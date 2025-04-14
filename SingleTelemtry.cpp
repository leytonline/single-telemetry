#include "SingleTelemetry.h"

static FILE* _output = nullptr;
static std::mutex _mutex;

int SingleTelemetry::SetFile(std::string str) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_output != NULL)
    {
        return -EEXIST;
    }

    _output = fopen(str.c_str(), "w");

    return EXIT_SUCCESS;
}

void SingleTelemetry::Write(std::string str) {
    std::lock_guard<std::mutex> lock(_mutex); // says fprintf is thread safe but just for exercise

    // some logs will get dropped if this tries to acquire the lock
    // and some other thread has called close
    if (_output == nullptr) return; 

    fprintf(_output, "%s", str.c_str());
}

int SingleTelemetry::Close() {
    std::lock_guard<std::mutex> lock(_mutex);
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

    // single lock is acquired in this function,
    // if a different thread closes file while TimeFunction is running,
    // it may time and execute fn but not be able to log the result
    Write(ss.str().c_str());
}

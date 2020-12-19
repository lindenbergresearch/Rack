#include <common.hpp>
#include <asset.hpp>
#include <settings.hpp>
#include <chrono>
#include <mutex>


namespace rack {
namespace logger {


static FILE *outputFile = nullptr;
static FILE *outputConsole = nullptr;

static std::chrono::high_resolution_clock::time_point startTime;
static std::mutex logMutex;

static Level logLevel = TRACE_LEVEL;


void init() {
    startTime = std::chrono::high_resolution_clock::now();

    if (settings::devMode) {
        outputConsole = stdout;
    }

    outputFile = fopen(asset::logPath.c_str(), "w");
    if (!outputFile) {
        fprintf(stdout, "Could not open log at %s\n", asset::logPath.c_str());
    }
}


void destroy() {
    if (outputFile) {
        fclose(outputFile);
    }
}


static const char *const levelLabels[] = {
        "TRACE",
        "DEBUG",
        "INFO ",
        "WARN ",
        "FATAL"
};

static const int levelColors[] = {
        35,
        36,
        98,
        33,
        31
};


static void logVa(Level level, const char *filename, int line, const char *format, va_list args) {
    std::lock_guard<std::mutex> lock(logMutex);

    auto nowTime = std::chrono::high_resolution_clock::now();
    int duration = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - startTime).count();

    if (outputConsole) {
        fprintf(outputConsole, "\x1B[%dm", levelColors[level]);
        fprintf(outputConsole, "\x1B[0m");
        fprintf(outputConsole, "[%00006.03f %-26s %4d] %s ", duration / 1000.0, filename, line, levelLabels[level]);
        vfprintf(outputConsole, format, args);
        fprintf(outputConsole, "\n");
        fflush(outputConsole);
    }

    fprintf(outputFile, "[%00006.03f %-26s %4d] %s ", duration / 1000.0, filename, line, levelLabels[level]);
    vfprintf(outputFile, format, args);
    fprintf(outputFile, "\n");
    fflush(outputFile);
}


void log(Level level, const char *filename, int line, const char *format, ...) {
    if (logLevel > level) return;

    va_list args;
    va_start(args, format);
    logVa(level, filename, line, format, args);
    va_end(args);
}


} // namespace logger
} // namespace rack

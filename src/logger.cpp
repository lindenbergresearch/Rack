#include <common.hpp>
#include <asset.hpp>
#include <settings.hpp>
#include <mutex>
#include <timer.hpp>


namespace rack {
namespace logger {


static FILE *outputFile = nullptr;
static FILE *outputConsole = nullptr;

static std::mutex logMutex;
static Level logLevel = TRACE_LEVEL;


void init() {
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
        31,
        36,
        39,
        33,
        31
};


static void logVa(Level level, const char *filename, int line, const char *funct, const char *format, va_list args) {
    std::lock_guard<std::mutex> lock(logMutex);

    long duration = rack::timer::nanoTime();

    if (outputConsole) {
        fprintf(outputConsole, "\x1B[%dm", levelColors[level]);
        fprintf(outputConsole, "[%-6.5f] <%s->%s:%d> %s ", (double) duration / 10e8, filename, funct, line, levelLabels[level]);
        vfprintf(outputConsole, format, args);
        fprintf(outputConsole, "\x1B[0m");
        fprintf(outputConsole, "\n");
        fflush(outputConsole);
    }

    fprintf(outputFile, "[%-6.5f] <%s->%s(...):%d> %s ", (double) duration / 10e8, filename, funct, line, levelLabels[level]);
    vfprintf(outputFile, format, args);
    fprintf(outputFile, "\n");
    fflush(outputFile);
}


void log(Level level, const char *filename, int line, const char *funct, const char *format, ...) {
    if (logLevel > level) return;

    va_list args;
    va_start(args, format);
    logVa(level, filename, line, funct, format, args);
    va_end(args);
}


} // namespace logger
} // namespace rack

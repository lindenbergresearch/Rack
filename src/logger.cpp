#include <common.hpp>
#include <asset.hpp>
#include <settings.hpp>
#include <chrono>
#include <mutex>


namespace rack {
namespace logger {


static FILE* outputFile = NULL;
static std::chrono::high_resolution_clock::time_point startTime;
static std::mutex logMutex;


void init() {
	startTime = std::chrono::high_resolution_clock::now();
	if (settings::devMode) {
		outputFile = stdout;
		return;
	}

	outputFile = fopen(asset::logPath.c_str(), "w");
	if (!outputFile) {
		fprintf(stdout, "Could not open log at %s\n", asset::logPath.c_str());
	}
}

void destroy() {
	if (outputFile != stdout) {
		fclose(outputFile);
	}
}

static const char* const levelLabels[] = {
	"DEBUG",
	"INFO ",
	"WARN ",
	"FATAL"
};

static const int levelColors[] = {
	36,
	97,
	33,
	31
};

static void logVa(Level level, const char* filename, int line, const char* format, va_list args) {
	std::lock_guard<std::mutex> lock(logMutex);

	auto nowTime = std::chrono::high_resolution_clock::now();
	int duration = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - startTime).count();

	if (outputFile == stdout) fprintf(outputFile, "\x1B[%dm", levelColors[level]);

	fprintf(outputFile, "[%00006.03f %-26s %4d] %s ", duration / 1000.0, filename, line, levelLabels[level]);

	vfprintf(outputFile, format, args);
    if (outputFile == stdout) fprintf(outputFile, "\x1B[0m");
	fprintf(outputFile, "\n");
	fflush(outputFile);
}

void log(Level level, const char* filename, int line, const char* format, ...) {
	va_list args;
	va_start(args, format);
	logVa(level, filename, line, format, args);
	va_end(args);
}


} // namespace logger
} // namespace rack

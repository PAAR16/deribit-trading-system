#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>

class Logger {
public:
    enum class Level { INFO, WARN, ERROR };

    static void log(Level level, const std::string& message) {
        std::cout << "[" << levelToString(level) << "] " << message << std::endl;
    }

private:
    static std::string levelToString(Level level) {
        switch (level) {
            case Level::INFO: return "INFO";
            case Level::WARN: return "WARN";
            case Level::ERROR: return "ERROR";
        }
        return "UNKNOWN";
    }
};

#endif // LOGGER_HPP
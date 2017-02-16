//
// Created by Chang-Soon Park on 2/14/17.
//

#ifndef ORDERBOOK_LOGGING_H
#define ORDERBOOK_LOGGING_H

#include <iomanip>
#include <iostream>
#include <unordered_map>

namespace baselineorderbook {

    class Logging {
    public:
        enum class LogLevel {
            // GLOBAL follows the global option, which itself is INFO by default.
                    GLOBAL=-1, DEBUG=0, INFO=10, WARNING=20, ERROR=30
        };

    private:
        Logging(const Logging&) = delete;
        Logging(Logging&&) = delete;
        Logging& operator= (const Logging&) = delete;
        Logging& operator= (Logging&&) = delete;

        LogLevel logLevel;
        std::string title;

        std::ostream& printDateTime(std::ostream& os) {
            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);
            os << std::put_time(&tm, "%c");
            return os;
        }

        void out(LogLevel level, const char* levelstr, const char* str) {
            LogLevel lvl = logLevel == LogLevel::GLOBAL ? globalLogLevel() : logLevel;
            if(level >= lvl) {
                std::cout << levelstr;
                std::cout << title << ": ";
                printDateTime(std::cout) << ": " << str << std::endl;
            }
        }

        std::ostream& out(std::ostream& os, LogLevel level, const char* levelstr) {
            LogLevel lvl = logLevel == LogLevel::GLOBAL ? globalLogLevel() : logLevel;
            if(level >= lvl) {
                os << levelstr;
                std::cout << title << ": ";
                printDateTime(os) << ": ";
            }
            return os;
        }

    public:
        // has to be public to use map::emplace in getLogger. any workaround?
        Logging(LogLevel logLevel, std::string title) : logLevel {logLevel}, title {title} {}

        static Logging& getLogger(const char* title) {
            return getLogger(LogLevel::GLOBAL, std::string(title));
        }

        static Logging& getLogger(std::string title) {
            return getLogger(LogLevel::GLOBAL, std::string(title));
        }

        static Logging& getLogger(LogLevel logLevel, const char* title) {
            return getLogger(logLevel, std::string(title));
        }

        static Logging& getLogger(LogLevel logLevel, std::string title) {
            static std::unordered_map<std::string, Logging> loggerMap {};

            auto it = loggerMap.find(title);
            if(it == loggerMap.end()) {
                loggerMap.emplace(std::piecewise_construct,
                                  std::forward_as_tuple(title),
                                  std::forward_as_tuple(logLevel, title));
                return loggerMap.at(title);
            }
            else {
                it->second.setLevel(logLevel);
                return it->second;
            }
        }
        void debug(const char* str) { return out(LogLevel::DEBUG, "[DEBUG] ", str); }
        void info(const char* str) { return out(LogLevel::INFO, "[INFO] ", str); }
        void warning(const char* str) { return out(LogLevel::WARNING, "[WARNING] ", str); }
        void error(const char* str) { return out(LogLevel::ERROR, "[ERROR] ", str); }

        std::ostream& debug(std::ostream& os) { return out(os, LogLevel::DEBUG, "[DEBUG] "); }
        std::ostream& info(std::ostream& os) { return out(os, LogLevel::INFO, "[INFO] "); }
        std::ostream& warning(std::ostream& os) { return out(os, LogLevel::WARNING, "[WARNING] "); }
        std::ostream& error(std::ostream& os) { return out(os, LogLevel::ERROR, "[ERROR] "); }

        void setLevel(LogLevel newLogLevel) { logLevel = newLogLevel; }
        LogLevel getLevel() { return logLevel; }

        static LogLevel& globalLogLevel() {
            static LogLevel globalLL = LogLevel::INFO;
            return globalLL;
        };
    };
}

#endif //ORDERBOOK_LOGGING_H

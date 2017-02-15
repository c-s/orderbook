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
        virtual ~Logging() {}
        virtual void debug(const char* str) = 0;
        virtual void info(const char* str) = 0;
        virtual void warning(const char* str) = 0;
        virtual void error(const char* str) = 0;

        virtual std::ostream& debug(std::ostream& os) = 0;
        virtual std::ostream& info(std::ostream& os) = 0;
        virtual std::ostream& warning(std::ostream& os) = 0;
        virtual std::ostream& error(std::ostream& os) = 0;

    public:
        enum class LogLevel {
            // GLOBAL follows the global option, which itself is INFO by default.
            GLOBAL=-1, DEBUG=0, INFO=10, WARNING=20, ERROR=30
        };
    };

    class SimpleLogging : public Logging {
    private:
        SimpleLogging(LogLevel logLevel, std::string title) : logLevel {logLevel}, title {title} {}
        SimpleLogging(const SimpleLogging&) = delete;
        SimpleLogging(SimpleLogging&&) = delete;
        SimpleLogging& operator= (const SimpleLogging&) = delete;
        SimpleLogging& operator= (SimpleLogging&&) = delete;

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
        static SimpleLogging& getLogger(const char* title) {
            return getLogger(LogLevel::GLOBAL, std::string(title));
        }

        static SimpleLogging& getLogger(std::string title) {
            return getLogger(LogLevel::GLOBAL, std::string(title));
        }

        static SimpleLogging& getLogger(LogLevel logLevel, const char* title) {
            return getLogger(logLevel, std::string(title));
        }

        static SimpleLogging& getLogger(LogLevel logLevel, std::string title) {
            static std::unordered_map<std::string, std::unique_ptr<SimpleLogging>> loggerMap {};

            auto it = loggerMap.find(title);
            if(it == loggerMap.end()) {
                auto logger = std::unique_ptr<SimpleLogging>(new SimpleLogging(logLevel, title));
                loggerMap[title] = std::move(logger);
                return *loggerMap[title];
            }
            else {
                it->second->setLevel(logLevel);
                return *it->second;
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

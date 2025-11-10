// Copyright (c) 2025 Josh Sellers
// Licensed under the MIT License. See LICENSE file.

#ifndef _LOGGER_H
#define _LOGGER_H

#include <string>
#include <queue>
#include <thread>
#include <fstream>
#include <filesystem>
#include <iostream>
#include "Util.h"
#include "../PennyEngine.h"


namespace pe {
    constexpr float LOG_WRITE_INTERVAL_SECONDS = 1.f;

    class Logger {
    public:
        static void start() {
            if (_isStarted) return;

            _isStarted = true;

            _logFileName = PennyEngine::getAppName() + ".log";
            _isHalted = false;

            try {
                std::filesystem::remove(_logFileName);
            } catch (const std::filesystem::filesystem_error& err) {
                std::cout << "Could not remove log file: " << err.what() << std::endl;
            }

            try {
                _outStream.open(_logFileName);
            } catch (std::exception ex) {
                std::cout << "Logging error: " << ex.what() << std::endl;
            }

            std::thread loggingThread(Logger::run);
            loggingThread.detach();
        }

        static void stop() {
            _isStarted = false;
            _isHalted = true;
            _outStream.close();
        }

        static void log(std::string message) {
            std::cout << message << std::endl;

            const auto now = std::chrono::system_clock::now();
            const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
            #pragma warning(suppress : 4996)
            std::string timeString = ctime(&t_c);
            timeString = timeString.substr(0, timeString.length() - 5);
            timeString += "@" + std::to_string(currentTimeMillis());

            message = "(" + timeString + (std::string)") " + message;

            if (!_isWriting) _messageQueue.push(message);
            else _deferredMessageQueue.push(message);
        }

        static void log(int message) {
            log(std::to_string(message));
        }

        static void log(float message) {
            log(std::to_string(message));
        }

        static void log(unsigned int message) {
            log(std::to_string(message));
        }

        static void log(long long message) {
            log(std::to_string(message));
        }

        static void log(double message) {
            log(std::to_string(message));
        }

        static void log(sf::Vector2f message) {
            log(std::to_string(message.x) + ", " + std::to_string(message.y));
        }

        static void log(sf::Vector2i message) {
            log(std::to_string(message.x) + ", " + std::to_string(message.y));
        }

        static const std::string getLogFileName() {
            return _logFileName;
        }

        static bool queuesHaveFlushed() {
            return _messageQueue.empty() && _deferredMessageQueue.empty();
        }

    private:
        inline static std::queue<std::string> _messageQueue;
        inline static std::queue<std::string> _deferredMessageQueue;

        inline static bool _isStarted = false;
        inline static bool _isHalted = false;
        inline static bool _isWriting = false;

        inline static std::string _logFileName = "PennyEngine.log";
        inline static std::ofstream _outStream;

        static void run() {
            while (!_isHalted) {
                std::this_thread::sleep_for(std::chrono::milliseconds((int)(LOG_WRITE_INTERVAL_SECONDS * 1000.f)));

                _isWriting = true;
                try {
                    while (!_messageQueue.empty()) {
                        _outStream << _messageQueue.front() << std::endl;
                        _messageQueue.pop();
                    }
                    _isWriting = false;

                    while (!_deferredMessageQueue.empty()) {
                        _outStream << _deferredMessageQueue.front() << std::endl;
                        _deferredMessageQueue.pop();
                    }

                    _outStream.flush();

                } catch (std::exception ex) {
                    std::cout << "Logging error: " << ex.what() << std::endl;
                }
            }
        }
    };
}

#endif
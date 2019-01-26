//
// Created by pux19 on 1/25/2019.
//

#ifndef GGJ_2019_LOGGER_H
#define GGJ_2019_LOGGER_H

#include <string>

class Logger {
public:
    static void Info(const std::string& log);
    static void Debug(const std::string& log);
    static void Error(const std::string& log);
};


#endif //GGJ_2019_LOGGER_H

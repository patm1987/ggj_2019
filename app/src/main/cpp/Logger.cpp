//
// Created by pux19 on 1/25/2019.
//

#include <android/log.h>
#include "Logger.h"

void Logger::Info(const std::string &log) {
    __android_log_write(ANDROID_LOG_INFO, "ggj_2019", log.c_str());
}

void Logger::Debug(const std::string &log) {
    __android_log_write(ANDROID_LOG_DEBUG, "ggj_2019", log.c_str());
}

void Logger::Error(const std::string &log) {
    __android_log_write(ANDROID_LOG_ERROR, "ggj_2019", log.c_str());
}

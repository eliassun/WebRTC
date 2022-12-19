//
// Created by Elias Sun on 12/01/22.
//

#ifndef SIP4WEBRTC_LOGGER_H
#define SIP4WEBRTC_LOGGER_H

#include <string>
#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <stdarg.h>

// TODO: support the logger for Apple iPhone MacOS

#define LOG_LEVEL_VERBOSE   1
#define LOG_LEVEL_DEBUG     2
#define LOG_LEVEL_INFO      3
#define LOG_LEVEL_WARNING   4
#define LOG_LEVEL_ERROR     5
#define LOG_LEVEL_LOCAL     6
#define LOG_LEVEL_DISABLED  -1


#define logv(...) Log::log(LOG_LEVEL_DEBUG  , __FILE__, __LINE__, __GUID_123_FUNCTION_NAME__, SIPTAG, "", 0, __VA_ARGS__)
#define logd(...) Log::log(LOG_LEVEL_DEBUG  , __FILE__, __LINE__, __GUID_123_FUNCTION_NAME__, SIPTAG, "", 0, __VA_ARGS__)
#define logi(...) Log::log(LOG_LEVEL_INFO   , __FILE__, __LINE__, __GUID_123_FUNCTION_NAME__, SIPTAG, "", 0, __VA_ARGS__)
#define logw(...) Log::log(LOG_LEVEL_WARNING, __FILE__, __LINE__, __GUID_123_FUNCTION_NAME__, SIPTAG, "", 0, __VA_ARGS__)
#define loge(...) Log::log(LOG_LEVEL_ERROR  , __FILE__, __LINE__, __GUID_123_FUNCTION_NAME__, SIPTAG, "", 0, __VA_ARGS__)
#define logm(...) Log::log(LOG_LEVEL_MUST  , __FILE__, __LINE__, __GUID_123_FUNCTION_NAME__, SIPTAG, "", 0, __VA_ARGS__)
#define pjerr(DESC, CODE, ...) Log::log(SIPSTACK_LEVEL_ERROR_REMOTE  , __FILE__, __LINE__, __GUID_123_FUNCTION_NAME__, SIPTAG, DESC, CODE, __VA_ARGS__)

#if defined(__ANDROID__)
#include <android/log.h>
#define TAG "sip4webrtcdroid"
#define LOG_ERROR ANDROID_LOG_ERROR
#define BUF_SIZE 10240
extern void addlog(int level, const char *file, int line, const char *func, const char *tag, const char *desc, int code, char *txt);
#endif

static void espjlog(int logLevel, const char *file, int line, const char *func, const char *tag, const char *desc, int code, const char *fmt, ...)
{
    va_list arglist;
    va_start(arglist, fmt);
    int level = logLevel;
    char buf[BUF_SIZE] = {0};
    if (level > LOG_LEVEL_LOCAL) {
        level = LOG_LEVEL_LOCAL;
    }
    vsnprintf(buf, BUF_SIZE, fmt, arglist);
    addlog(logLevel, file, line, func, tag, desc, code, buf);
    va_end(arglist);
}

#endif //SIP4WEBRTC_LOGGER_H

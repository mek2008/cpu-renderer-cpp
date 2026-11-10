#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <cstdio>

#ifdef DEBUG
    #define DEBUG_LOG(...) \
        do { fprintf(stderr, __VA_ARGS__); } while (0)

    #define DEBUG_LOG_IF(condition, ...) \
        do { if(condition) fprintf(stderr, __VA_ARGS__); } while (0)
#else
    #define DEBUG_LOG(...)
    #define DEBUG_LOG_IF(condition, ...)
#endif

#endif
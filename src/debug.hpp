#ifndef DEBUG_HPP
#define DEBUG_HPP

#ifdef DEBUG
    #include <cstdio>
    #define DEBUG_LOG(...) fprintf(stderr, __VA_ARGS__)
    #define DEBUG_LOG_IF(condition, ...) 
        do { if (condition) fprintf(stderr, __VA_ARGS__); } while (0)
#else
    #define DEBUG_LOG(...) ((void)0)
    #define DEBUG_LOG_IF(condition, ...) ((void)0)
#endif

#endif
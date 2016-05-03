#ifndef __ERROR_HPP__
#define __ERROR_HPP__

#include <cstdio>
#include <iostream>
#include <utility>

using namespace std;

#define DEBUG_ENABLE   (1)

#ifndef __cplusplus

#define DEBUG(fmt, ...) \
    do { if (DEBUG_ENABLE) fprintf(stderr, "[DEBUG] %s:%d:%s(): " fmt "\n", \
        __FILE__, __LINE__, __func__, ##__VA_ARGS__); } while (0)
#define ERROR(fmt, ...) \
    do { if (DEBUG_ENABLE) fprintf(stderr, "[ERROR] %s:%d:%s(): " fmt "\n", \
        __FILE__, __LINE__, __func__, ##__VA_ARGS__); } while (0)
#define LOG(fmt, ...) \
    do { if (DEBUG_ENABLE) fprintf(stderr, "[LOG  ] %s:%d:%s(): " fmt "\n", \
        __FILE__, __LINE__, __func__, ##__VA_ARGS__); } while (0)

#else    // __cplusplus

inline void __multiprint() { }

template<typename First, typename ...Rest>
inline void __multiprint(First && first, Rest && ...rest)
{
    cerr << " " << forward<First>(first);
    __multiprint(forward<Rest>(rest)...);
}

#define DEBUG(...) \
    do { if (DEBUG_ENABLE) cerr << "[DEBUG] " << __FILE__ << ":" <<  \
        __LINE__ << ":" << __func__ << "():"; __multiprint(__VA_ARGS__); \
        cerr << endl; } while (0)

#define ERROR(...) \
    do { if (DEBUG_ENABLE) cerr << "[ERROR] " << __FILE__ << ":" <<  \
        __LINE__ << ":" << __func__ << "():"; __multiprint(__VA_ARGS__); \
        cerr << endl; } while (0)

#define LOG(...) \
    do { if (DEBUG_ENABLE) cerr << "[LOG  ] " << __FILE__ << ":" <<  \
        __LINE__ << ":" << __func__ << "():"; __multiprint(__VA_ARGS__); \
        cerr << endl; } while (0)

#endif    // __cplusplus

#endif    // __ERROR_HPP__

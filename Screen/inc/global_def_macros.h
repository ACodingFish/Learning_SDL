#ifndef _GLOBAL_DEF_MACROS_H_
#define _GLOBAL_DEF_MACROS_H_

#define MASK_BIT_SET(m, b) (m |= (1<<(b)))
#define MASK_BIT_CLR(m, b) (m &= (~(1<<(b))))
#define MASK_BIT_TOG(m, b) (m ^= (1<<(b)))

#define MASK_BIT_CHK(m, b) ((m & (1<<(b))) > 0)

#define ENABLE_DEBUG_MODE 3 // 3 (Log), 2 (Warnings), 1 (Error)

#ifdef ENABLE_DEBUG_MODE
#   define DBG_LOG(...)     do { \
        if (ENABLE_DEBUG_MODE >= 3){ \
            printf("[DBG] %s, function %s: ", __FILE__, __FUNCTION__); \
            printf(__VA_ARGS__); \
        } \
    } while(0)
#   define DBG_WARN(...)     do { \
        if (ENABLE_DEBUG_MODE >= 2){ \
            printf("[WARN] %s, function %s: ", __FILE__, __FUNCTION__); \
            printf(__VA_ARGS__); \
        } \
    } while(0)
#   define DBG_ERR(...)     do { \
        if (ENABLE_DEBUG_MODE >= 1){ \
            printf("[ERR] %s, function %s: ", __FILE__, __FUNCTION__); \
            printf(__VA_ARGS__); \
        } \
    } while(0)
#else
#   define DBG_LOG(...)
#   define DBG_WARN(...)
#   define DBG_ERR(...)
#endif

#endif //_GLOBAL_DEF_MACROS_H_
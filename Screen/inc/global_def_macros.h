#ifndef _GLOBAL_DEF_MACROS_H_
#define _GLOBAL_DEF_MACROS_H_

#define MASK_BIT_SET(m, b) (m |= (1<<(b)))
#define MASK_BIT_CLR(m, b) (m &= (~(1<<(b))))
#define MASK_BIT_TOG(m, b) (m ^= (1<<(b)))

#define MASK_BIT_CHK(m, b) ((m & (1<<(b))) > 0)


#define ENABLE_DEBUG_MODE
#ifdef ENABLE_DEBUG_MODE
#   define DBG_LOG(...)     do { \
        printf("[DBG] %s, function %s: ", __FILE__, __FUNCTION__); \
        printf(__VA_ARGS__); \
    } while(0)
#   define DBG_ERR(...)     do { \
        printf("[ERR] %s, function %s: ", __FILE__, __FUNCTION__); \
        printf(__VA_ARGS__); \
    } while(0)
#else
#   define DBG_LOG(...)
#   define DBG_ERR(...)
#endif

#endif //_GLOBAL_DEF_MACROS_H_
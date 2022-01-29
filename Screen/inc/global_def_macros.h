#ifndef _GLOBAL_DEF_MACROS_H_
#define _GLOBAL_DEF_MACROS_H_

#define MASK_BIT_SET(m, b) (m |= (1<<(b)))
#define MASK_BIT_CLR(m, b) (m &= (~(1<<(b))))
#define MASK_BIT_TOG(m, b) (m ^= (1<<(b)))

#define MASK_BIT_CHK(m, b) ((m & (1<<(b))) > 0)


#define ENABLE_DEBUG_MODE
#ifdef ENABLE_DEBUG_MODE
#   define DBG_LOG(...)     printf("[DBG] %s, function %s: %s\n", __FILE__, __FUNCTION__, __VA_ARGS__);
#else
#   define DBG_LOG(...)
#endif

#endif //_GLOBAL_DEF_MACROS_H_
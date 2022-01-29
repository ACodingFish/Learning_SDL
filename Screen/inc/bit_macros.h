#ifndef _BIT_MACROS_H_
#define _BIT_MACROS_H_

#define MASK_BIT_SET(m, b) (m |= (1<<(b)))
#define MASK_BIT_CLR(m, b) (m &= (~(1<<(b))))
#define MASK_BIT_TOG(m, b) (m ^= (1<<(b)))

#define MASK_BIT_CHK(m, b) ((m & (1<<(b))) > 0)

#endif //_BIT_MACROS_H_
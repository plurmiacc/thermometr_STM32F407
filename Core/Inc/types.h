#ifndef INC_TYPES_H_
#define INC_TYPES_H_

#include "stdint.h"

typedef enum {
    FALSE,
    TRUE = !FALSE,
} BOOL __attribute__ ((mode(QI)));

#ifndef NULL
#define NULL ((void*)0)
#endif // NULL

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif // UNUSED

typedef enum {
	Result_OK,
	Result_FAIL,
	Result_ERROR,
	Result_BADPARAM,
	Result_NULLPTR,
	//
	Result_NUM,
}Result_t __attribute__ ((mode(QI)));

#endif /* INC_TYPES_H_ */

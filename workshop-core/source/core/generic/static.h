#include "non_copyable.h"
#include "non_movable.h"

#define WK_STATIC_CLASS(ClassName)			\
	ClassName() = delete;				\
	NON_COPYABLE(ClassName)				\
	NON_MOVABLE(ClassName)
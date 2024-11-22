#pragma once

#define NON_MOVABLE(ClassName)                  \
	ClassName(ClassName&&) = delete;            \
	ClassName& operator=(ClassName&&) = delete; \

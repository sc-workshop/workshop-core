#include "core/exception/exception.h"

#include <stdio.h>

namespace wk
{
	Exception::Exception(const char* format, ...)
	{
		va_list args;
		va_start(args, format);

		proceed_vargs(format, args);

		va_end(args);
	}

	void Exception::proceed_vargs(const char* format, va_list args)
	{
		char message[2048]; // TODO(pavidloq): wtf?
        vsnprintf(message, sizeof(message), format, args);
		m_message = message;
	}
}

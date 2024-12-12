#include "shared_memory_stream.h"

namespace wk
{
	SharedMemoryStream::SharedMemoryStream(uint8_t* data, size_t length)
	{
		m_data = data;
		m_length = length;
	}

	void* SharedMemoryStream::data_ref() const
	{
		return m_data;
	}
}
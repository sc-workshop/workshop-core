#include "core/io/memory_stream.h"

#include "core/memory/memory.h"

namespace wk
{
	MemoryStream::MemoryStream(std::size_t length)
	{
		if (length == 0) return;

		m_data = Memory::allocate<T>(length);
		m_length = length;
	}

	MemoryStream::~MemoryStream() {
		Memory::free(m_data);
	}

	void* MemoryStream::data_ref() const { return (void*)m_data; };
}

#include "core/io/memory_stream.h"

#include "core/memory/memory.h"

namespace wk
{
	MemoryStream::MemoryStream(std::size_t length)
	{
		if (length == 0) return;

		m_data = TPtr(Memory::allocate<T>(length));
		m_length = length;
	}

	MemoryStream::MemoryStream(const MemoryStream& other) : m_data(other.get_reference())
	{
	}

	void* MemoryStream::data_ref() const { return (void*)m_data.get(); };

	MemoryStream::TPtr MemoryStream::get_reference() const{ return m_data; }
}

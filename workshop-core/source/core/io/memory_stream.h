#pragma once

#include "base_memory_stream.h"

namespace wk
{
	class WORKSHOP_API MemoryStream : public BaseMemoryStream
	{
	private:
		using T = uint8_t;
		using TPtr = Ref<T>;

	public:
		MemoryStream(std::size_t length = 0);
		virtual ~MemoryStream();

	public:
		virtual void* data_ref() const;

	private:
		T* m_data;
	};
}

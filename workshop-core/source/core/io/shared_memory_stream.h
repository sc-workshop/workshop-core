#pragma once

#include "base_memory_stream.h"

namespace wk
{
	class WORKSHOP_API SharedMemoryStream : public BaseMemoryStream
	{
	public:
		SharedMemoryStream(uint8_t* data, size_t length);

		virtual ~SharedMemoryStream() = default;

	protected:
		virtual void* data_ref() const;

	private:
		uint8_t* m_data;
	};
}

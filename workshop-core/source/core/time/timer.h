#pragma once

#include "core/preprocessor/api.h"

#include <chrono>

namespace wk
{
	class WORKSHOP_API Timer
	{
	public:
		Timer()
		{
			reset();
		}

		void reset()
		{
			m_start_time = std::chrono::high_resolution_clock::now();
		}

		long long elapsed() const
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_start_time).count();
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;
	};
}
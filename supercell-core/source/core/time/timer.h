#pragma once

#include <chrono>
#include "core/preprocessor/api.h"

namespace sc
{
	class SUPERCELL_API Timer
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

		float elapsed()
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_start_time).count();
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;
	};
}
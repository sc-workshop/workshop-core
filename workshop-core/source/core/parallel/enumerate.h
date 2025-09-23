#pragma once

#include <functional>
#include <vector>
#include <future>

namespace wk::parallel
{
	template<std::input_iterator Iterator, typename Func>
	inline void enumerate(
		Iterator from, Iterator to,
		Func&& fn,
		std::launch policy = std::launch::deferred | std::launch::async)
	{
		size_t task_count = std::distance(from, to);

		std::vector<std::future<void>> tasks(task_count);

		auto it = from;
		for (size_t idx = 0; idx < task_count; idx++) {
			tasks[idx] = std::async(policy, fn, std::ref(*it++), idx);
		}

		for (size_t i = 0; i < task_count; ++i) tasks[i].wait();

	}
}
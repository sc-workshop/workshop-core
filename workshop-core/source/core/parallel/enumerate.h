#pragma once

#include <functional>
#include <vector>
#include <future>

namespace wk::parallel
{
	template<class T>
	using TIteratorValue = typename std::iterator_traits<T>::value_type;

	template<class Iterator>
	inline void enumerate(
		Iterator from, Iterator to,
		std::function<void(TIteratorValue<Iterator>&, size_t)> fn,
		std::launch policy = std::launch::deferred | std::launch::async)
	{
		size_t task_count = std::distance(from, to);

		std::vector<std::future<void>> tasks(task_count);

		auto it = from;
		for (size_t i = 0; i < task_count; i++) {
			tasks[i] = std::async(policy, fn, std::ref(*it++), unsigned(i));
		}

		for (size_t i = 0; i < task_count; ++i) tasks[i].wait();

	}
}
#include "thread_pool.h"

namespace wk {
	ThreadPool::ThreadPool(size_t count)
	{
		setup_threads(count);
	}

	ThreadPool::~ThreadPool()
	{
		stop = true;
		for (auto& thread : m_threads) {
			thread.join();
		}
	}

	void ThreadPool::setup_threads(size_t count)
	{
		std::lock_guard guard(mut);

		m_threads.clear();
		m_threads.reserve(count);

		for (size_t i = 0; count > i; i++) {
			m_threads.emplace_back(ThreadPool::worker_function, std::ref(*this));
		}
	}

	void ThreadPool::worker_function(ThreadPool& pool)
	{
		while (!pool.stop) {
			std::function<void()> fn;
			{
				std::unique_lock guard(pool.mut);
				pool.m_condition.wait(guard, [&pool]() {
					return !pool.m_tasks.empty();
				});

				fn = std::move(pool.m_tasks.front());
				pool.m_tasks.pop();
			}

			fn();
		}
	}
}


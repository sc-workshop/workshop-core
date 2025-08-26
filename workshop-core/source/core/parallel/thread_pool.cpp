#include "thread_pool.h"

namespace wk {
	ThreadPool::ThreadPool(size_t count)
	{
		setup_threads(count);
	}

	ThreadPool::~ThreadPool()
	{
		if (!stop)
			join();
	}

	void ThreadPool::setup_threads(size_t count)
	{
		std::lock_guard guard(mut);

		stop = false;
		m_threads.clear();
		m_threads.reserve(count);

		for (size_t i = 0; count > i; i++) {
			auto& thread = m_threads.emplace_back();
			thread.handle = std::thread(ThreadPool::worker_function, std::ref(*this), std::ref(thread));
		}
	}

	void ThreadPool::worker_function(ThreadPool& pool, ThreadContext& context)
	{
		while (true) {
			std::function<void()> fn;
			{
				std::unique_lock guard(pool.mut);
				pool.m_input.wait(guard, [&pool]() {
					return pool.should_stop() || !pool.m_tasks.empty();
				});

				if (pool.stop && pool.m_tasks.empty()) {
					return;
				}
					

				fn = std::move(pool.m_tasks.front());
				context.active = true;
				pool.m_tasks.pop();
			}

			try {
				fn();
			}
			catch (...) {
				pool.m_exception = std::current_exception();
				return;
			}

			context.active = false;
			pool.m_output.notify_all();
		}
	}

	bool ThreadPool::should_stop()
	{
		return stop || m_exception;
	}

	void ThreadPool::handle_pool_break()
	{
		if (m_exception) {
			std::rethrow_exception(m_exception);
		}
		else {
			throw wk::Exception("Pool is stopped and cannot be used again");
		}
	}

	size_t ThreadPool::active_threads() const
	{
		size_t result = 0;
		for (const auto& thread : m_threads) {
			if (thread.active)
				result++;
		}
		return result;
	}

	void ThreadPool::join() {
		stop = true;
		m_input.notify_all();
		for (auto& thread : m_threads) {
			thread.handle.join();
		}
	}

	void ThreadPool::wait() {
		if (m_tasks.empty()) return;

		std::unique_lock guard(mut);
		m_output.wait(guard, [&]() {
			if (should_stop())
				handle_pool_break();

			return m_tasks.empty() && active_threads() == 0;
		});
	}

	size_t ThreadPool::size() const {
		return m_threads.size();
	}

	void ThreadPool::resize(size_t count)
	{
		if (size() == count)
			return;

		join();
		setup_threads(count);
	}
}


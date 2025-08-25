#pragma once

#include <mutex>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <condition_variable>
#include <iostream>

namespace wk {
	class ThreadPool {
	public:
		ThreadPool(size_t count);
		~ThreadPool();

	public:
		template<typename Fn, typename... Args> 
		void push(Fn& fn, Args&... args){
			auto callable = [=]() {
				fn(args...);
			};

			std::lock_guard guard(mut);
			m_tasks.push(callable);
			m_condition.notify_one();
		}

	private:
		void setup_threads(size_t count);

		static void worker_function(ThreadPool& pool);

	private:
		std::mutex mut;

		std::vector<std::thread> m_threads;
		std::queue<std::function<void()>> m_tasks;
		std::condition_variable m_condition;
		bool stop = false;
	};
}
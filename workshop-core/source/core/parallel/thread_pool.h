#pragma once

#include <mutex>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <condition_variable>
#include <iostream>
#include "core/memory/ref.h"
#include "core/exception/exception.h"

namespace wk {
	struct ThreadContext {
		bool active = false;
		std::thread handle;
	};

	class ThreadPool {
	public:
		static inline size_t defaultThreadCount = std::thread::hardware_concurrency();

		// Global top-level pool
		static ThreadPool& Instance() {
			static ThreadPool instance;
			return instance;
		}

	public:
		ThreadPool(size_t count = ThreadPool::defaultThreadCount);
		~ThreadPool();

	public:
		template<typename Fn, typename... Args> 
		void push(Fn&& fn, Args&&... args){
			if (should_stop())
				handle_pool_break();

			auto callable = [=]() {
				fn(args...);
			};

			std::lock_guard guard(mut);
			m_tasks.push(callable);
			m_input.notify_one();
		}

		// Finish all the tasks and wait workers to exit
		void join();

		// Change count of worker threads
		void resize(size_t count);

		// Current coutn of worker threads
		size_t size() const;

		// Wait for all tasks to be finished
		void wait();

		// Count of working threads
		size_t active_threads() const;

	private:
		void setup_threads(size_t count);
		static void worker_function(ThreadPool& pool, ThreadContext& context);

		bool should_stop();
		void handle_pool_break();

	private:
		std::mutex mut;

		std::vector<ThreadContext> m_threads;
		std::queue<std::function<void()>> m_tasks;
		std::condition_variable m_input;
		std::condition_variable m_output;
		bool stop = false;
		
		std::exception_ptr m_exception;
	};
}
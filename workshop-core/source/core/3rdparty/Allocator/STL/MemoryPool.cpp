#include "core/3rdparty/Allocator/STL/MemoryPool.h"

#include <iostream>

namespace wk
{
	namespace STL
	{
		MemoryPool::MemoryPool() : MemoryPool(64, 1024) {}

		MemoryPool::MemoryPool(size_t n, size_t sz) : m_block_size(sz), m_block_num(n) {
			m_pool = operator new(m_pool_size);
			// succeess create the pool
			if (m_pool != nullptr) {
				for (size_t i = 0; i < m_block_num; ++i) {
					block_ptr curr = reinterpret_cast<block_ptr>(static_cast<char*>(m_pool) + i * (m_block_size + sizeof(block)));

					// insert curr into free blocks linked list
					// insert before
					curr->prev = nullptr;
					curr->next = m_free_block;

					if (m_free_block != nullptr) {
						m_free_block->prev = curr;
					}

					m_free_block = curr;
				}
			}
		}

		MemoryPool::~MemoryPool() {
			if (m_pool != nullptr)
				operator delete(m_pool);
		}

		void* MemoryPool::allocate(size_t sz) {
			// derectly use operator new
			if (sz > m_block_size || m_free_block == nullptr || m_pool == nullptr)
				return operator new(sz);

			//std::cout << "use pool" << std::endl;
			block_ptr curr = m_free_block;
			// delete the node from free block linked list
			m_free_block = curr->next;
			if (m_free_block != nullptr) {
				m_free_block->prev = nullptr;
			}

			// add node to allocated block linked list
			curr->next = m_alloc_block;
			if (m_alloc_block != nullptr) {
				m_alloc_block->prev = curr;
			}
			m_alloc_block = curr;

			return static_cast<void*>(reinterpret_cast<char*>(curr) + sizeof(block));
		}

		void MemoryPool::deallocate(void* ptr, size_t) {
			// pointer is inside memory pool
			if (m_pool < ptr && ptr < (void*)((char*)m_pool + m_pool_size)) {
				block_ptr curr = reinterpret_cast<block_ptr>(static_cast<char*>(ptr) - sizeof(block));

				// delete node to allocated block linked list
				m_alloc_block = curr->next;
				if (m_alloc_block != nullptr) {
					m_alloc_block->prev = nullptr;
				}

				// add the node from free block linked list
				curr->next = m_free_block;
				if (m_free_block != nullptr) {
					m_free_block->prev = curr;
				}
				m_free_block = curr;

				//std::cout << "use pool" << std::endl;
			}
			else
				operator delete(ptr);
		}
	}
}
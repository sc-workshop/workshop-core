// Source: https://github.com/ysmiles/allocator-cpp

#pragma once

#include <cstddef>
#include <limits>



namespace wk
{
	namespace STL
	{
		class MemoryPool {
		public:
			MemoryPool();
			MemoryPool(size_t n, size_t sz);
			void* allocate(size_t sz);
			void deallocate(void* ptr, size_t = 0);
			~MemoryPool();

		private:
			struct block {
				struct block* prev = nullptr, * next = nullptr;
			};
			typedef struct block* block_ptr;
			void* m_pool = nullptr; // address of the whole pool
			block_ptr m_free_block = nullptr; // free memory block linked list
			block_ptr m_alloc_block = nullptr; // alloced memory bleck linked list

			size_t m_block_size;
			size_t m_block_num;
			size_t m_pool_size = m_block_num * (m_block_size + sizeof(block));
		};

		/// <summary>
		/// Simple Memory Pool Allocator for STL containers
		/// </summary>
		/// <typeparam name="T"> Object Type </typeparam>
		/// <typeparam name="S"> Size Type </typeparam>
		template <typename T, typename S = std::size_t>
		class MemoryPoolAllocator {
		public:
			// naming tradition
			typedef T value_type;
			typedef T* pointer;
			typedef const T* const_pointer;
			typedef T& reference;
			typedef const T& const_reference;
			typedef S size_type;
			typedef std::ptrdiff_t difference_type;

			// for inner rebind michanics
			// used to transform allocator of T to tyep U
			template <typename U> struct rebind {
				typedef MemoryPoolAllocator<U, S>
					other;
			};

			// constructors and destructors
			MemoryPoolAllocator() = default;
			MemoryPoolAllocator(const MemoryPoolAllocator&) {}
			template <typename U> MemoryPoolAllocator(const MemoryPoolAllocator<U, S>&) {}
			// explicitly note that assignment operator is not needed
			MemoryPoolAllocator& operator=(const MemoryPoolAllocator&) = delete;
			~MemoryPoolAllocator() = default;

			// reuturn address of value_type
			static pointer address(reference r) {
				return &r;
			}
			static const_pointer address(const_reference cr) {
				return &cr;
			}

			// reutrn maximun number can be allocated
			static size_type max_size() {
				return std::numeric_limits<size_type>::max();
			}

			// operator used for compare
			bool operator==(const MemoryPoolAllocator&) const {
				return true;
			}
			bool operator!=(const MemoryPoolAllocator&) const {
				return false;
			}

			// implement by wrapping operator new and delete
			// allocation and deallocation
			static pointer allocate(size_type n) {
				// result will be copied to return value
				// use memory pool m_pool
				return static_cast<pointer>(memp.allocate(sizeof(T) * n));
			}
			static pointer allocate(size_type n, pointer) {
				return allocate(n);
			}
			static void deallocate(pointer ptr, size_type n) {
				// use memory pool m_pool
				memp.deallocate(ptr, n);
			}

			// constrution valu_type and destruction
			// still use new, here we just need to call the constructor
			static void construct(pointer ptr, const value_type& t) {
				new(ptr) value_type(t);
			}
			static void destroy(pointer ptr) {
				ptr->~value_type();
			}

		private:
			static MemoryPool memp;
		};

		template <typename T, typename S> MemoryPool MemoryPoolAllocator<T, S>::memp;
	}
}
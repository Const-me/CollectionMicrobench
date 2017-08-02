#pragma once
#include <array>
#include <stdint.h>

namespace PlexAlloc
{
	// Implements a single linked list of free elements, and exposes LIFO stack interface for that list.
	// While relatively efficient, strictly speaking, the implementation corrupts memory of the freed objects. Therefore, after FreeList::push(), calling ~T() destructor gonna cause a crash in runtime. If you want to call a destructor, do it before FreeList::push(). Fortunately, looks like STL containers do just that.
	template<class T>
	class FreeList
	{
		union FreeItemBuffer
		{
			FreeItemBuffer* next;
			std::array<uint8_t, sizeof( T )> storage;
		};
		FreeItemBuffer* m_top = nullptr;

	public:
		FreeList()
		{
			static_assert( sizeof( T ) >= sizeof( void* ), "FreeList is only compatible with elements larger than a pointer." );
			m_top = nullptr;
		}
		~FreeList() = default;
		FreeList( const FreeList& ) = delete;
		FreeList& operator=( const FreeList& ) = delete;
		FreeList( FreeList&& that ) : m_top( that.m_top )
		{
			that.m_top = nullptr;
		}
		FreeList& operator=( FreeList&& that )
		{
			m_top = that.m_top;
			that.m_top = nullptr;
			return *this;
		}

		void push( T* ptr )
		{
			auto node_ptr = reinterpret_cast<FreeItemBuffer*>( ptr );
			node_ptr->next = m_top;
			m_top = node_ptr;
		}

		bool empty() const
		{
			return nullptr == m_top;
		}

		T* pop()
		{
			T* res = reinterpret_cast<T*>( m_top );
			m_top = m_top->next;
			return res;
		}
	};
}
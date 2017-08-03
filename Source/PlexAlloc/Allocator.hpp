#pragma once
#include "Plex.hpp"
#include "FreeList.hpp"
#include "MemAlloc.hpp"

namespace PlexAlloc
{
	// CAtlPlex-like allocator compatible with node-based STL collections (linked lists, RB trees, hash maps).
	template <typename T, size_t nBlockSize = 10, size_t align = alignof ( T )>
	class Allocator
	{
		Plex<T, nBlockSize, align> m_plex;
		FreeList<T> m_freeList;

	public:
		using value_type = T;
		using size_type = std::size_t;
		using propagate_on_container_move_assignment = std::true_type;

		template<class U>
		struct rebind { using other = Allocator<U, nBlockSize, align>; };

		Allocator() noexcept {};

		Allocator( const Allocator& ) noexcept {};

		template <typename U, size_t bs, size_t a>
		Allocator( const Allocator<U, bs, a>& ) noexcept {};

		Allocator( Allocator&& other ) noexcept: m_plex( other.m_plex ), m_freeList( other.m_freeList ) { }

		Allocator& operator = ( const Allocator& ) noexcept
		{
			return *this;
		}

		Allocator& operator = ( Allocator&& other ) noexcept
		{
			m_freeList = other.m_freeList;
			m_plex = other.m_plex;
			return *this;
		}

		~Allocator() noexcept = default;

		T* allocate( size_type n )
		{
			if( 1 == n )
			{
				if( m_freeList.empty() )
					return m_plex.allocate();
				return m_freeList.pop();
			}
			// Fall back to malloc.
			// This is used in e.g. Microsoft's std::unordered_map, which holds both individual nodes of type _List_node<pair<K,V>>, and also a vector of _List_unchecked_iterator<_List_val<_List_simple_types<pair<K,V>>>>
			void* pArray = alignedMalloc( n * sizeof( T ), align );
			if( nullptr == pArray )
				throw std::bad_alloc();
			return static_cast<T*>( pArray );
		}

		void deallocate( T* ptr, size_type n ) noexcept
		{
			if( 1 == n )
				m_freeList.push( ptr );
			else
				alignedFree( ptr );
		}
	};

#ifdef _MSC_VER
	// In Microsoft's version of STL, when _ITERATOR_DEBUG_LEVEL is non-zero, each collection contains a dynamically-allocated pointer to a _Container_proxy instance.
	// We don't want to use PlexAlloc allocator for that one.
	template<size_t nBlockSize, size_t align>
	class Allocator<std::_Container_proxy, nBlockSize, align>
	{
	public:
		using value_type = std::_Container_proxy;

		Allocator() noexcept {};

		Allocator( const Allocator& ) noexcept = default;

		template <typename U, size_t bs, size_t a>
		Allocator( const Allocator<U, bs, a>& ) noexcept {}

		std::_Container_proxy* allocate( size_t n )
		{
			return static_cast<std::_Container_proxy*>( ::operator new( n * sizeof( std::_Container_proxy ) ) );
		}

		void deallocate( std::_Container_proxy* ptr, size_t n ) noexcept
		{
			::operator delete( ptr );
		}
	};
#endif
}
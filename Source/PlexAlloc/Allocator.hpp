#pragma once
#include "Plex.hpp"
#include "FreeList.hpp"
#include "MemAlloc.hpp"

namespace PlexAlloc
{
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

		Allocator( const Allocator& ) noexcept = delete;

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
			return static_cast<T*>( alignedMalloc( n * sizeof( T ), align ) );
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
	// In Microsoft's version of STL, each collection contains a dynamically-allocated pointer to a _Container_proxy instance.
	// We don't want to use PlexAlloc allocator for that one.
	template<>
	class Allocator<std::_Container_proxy>
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
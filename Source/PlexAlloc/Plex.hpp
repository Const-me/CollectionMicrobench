#pragma once
#include <array>
#include <type_traits>
#include "MemAlloc.hpp"

namespace PlexAlloc
{
	// This class allocates RAM in continuous batches, similar to CAtlPlex.
	// It only frees RAM when destroyed.
	// It doesn't call neither constructors nor destructors, and allocate() method returns uninitialized blocks of RAM.
	template <typename T, size_t nBlockSize = 10, size_t align = alignof ( T )>
	class Plex
	{
		struct CMemChunk
		{
			std::array<typename std::aligned_storage<sizeof( T ), align>::type, nBlockSize> items;
			CMemChunk* pNext;
		};

		CMemChunk* m_pChunk; // Most recently allocated a.k.a. the current chunk
		size_t m_usedInChunk; // Count of allocated items in the current chunk

		static CMemChunk* allocateChunk()
		{
			return (CMemChunk*)alignedMalloc( sizeof( CMemChunk ), align );
		}
		static void freeChunk( CMemChunk* p )
		{
			alignedFree( p );
		}

		void reset() noexcept
		{
			m_pChunk = nullptr;
			m_usedInChunk = nBlockSize;
		}

	public:
		Plex() { reset(); }
		Plex( const Plex & that ) = delete;
		Plex& operator=( const Plex & that ) = delete;
		Plex( Plex && that )
		{
			m_pChunk = that.m_pChunk;
			m_usedInChunk = that.m_usedInChunk;
			that.reset();
		}
		Plex& operator= ( Plex && that )
		{
			m_pChunk = that.m_pChunk;
			m_usedInChunk = that.m_usedInChunk;
			that.reset();
			return *this;
		}

		// Free all memory, don't call the destructors
		void clear()
		{
			while( nullptr != m_pChunk )
			{
				CMemChunk* pNext;
				pNext = m_pChunk->pNext;
				freeChunk( m_pChunk );
				m_pChunk = pNext;
			}
			reset();
		}

		// Allocate a new item, don't call a constructor
		T* allocate()
		{
			const size_t nNewCount = m_usedInChunk + 1;
			if( nNewCount <= nBlockSize )
			{
				T* const result = reinterpret_cast<T*>( &m_pChunk->items[ m_usedInChunk ] );
				m_usedInChunk = nNewCount;
				return result;
			}

			CMemChunk* const pNewChunk = allocateChunk();
			if( nullptr == pNewChunk )
				throw std::bad_alloc();

			pNewChunk->pNext = m_pChunk;
			m_pChunk = pNewChunk;
			m_usedInChunk = 1;
			return reinterpret_cast<T*>( pNewChunk->items.data() );
		}
	};
}
#pragma once

namespace PlexAlloc
{
	// Allocate aligned block of memory
	inline void* alignedMalloc( size_t size, size_t alignment )
	{
#ifdef _MSC_VER
		return _aligned_malloc( size, alignment );
#else
		return aligned_alloc( alignment, size );
#endif
	}

	// Free aligned block of memory
	inline void alignedFree( void* ptr )
	{
#ifdef _MSC_VER
		_aligned_free( ptr );
#else
		free( ptr );
#endif
	}
}
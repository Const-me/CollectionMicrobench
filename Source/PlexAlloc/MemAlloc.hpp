#pragma once

namespace PlexAlloc
{
	inline void* alignedMalloc( size_t size, size_t alignment )
	{
#ifdef _MSC_VER
		return _aligned_malloc( size, alignment );
#else
		return std::aligned_alloc( alignment, size )
#endif
	}

	inline void alignedFree( void* ptr )
	{
#ifdef _MSC_VER
		_aligned_free( ptr );
#else
		std::free( ptr );
#endif
	}
}
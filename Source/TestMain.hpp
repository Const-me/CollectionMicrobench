#pragma once
#include <chrono>
#include <stdio.h>
#include "PlexAlloc/Allocator.hpp"

template <typename T>
using PlexAllocator = PlexAlloc::Allocator<T>;

class iTestCase
{
public:
	virtual void prepare( size_t length ) = 0;
	virtual int run( int inserts ) = 0;
};

// A test case for STL containers. Because they expose very similar API, a single template implementation is required.
template<class tContainer>
class testStl: public iTestCase
{
	tContainer local;

	void prepare( size_t length ) override
	{
		for( size_t i = 0; i < length; i++ )
			local.push_back( 1 );
	}
	int run( int localInserts ) override
	{
		for( int i = 0; i < localInserts; i++ )
			local.insert( local.begin(), 1 );

		int sum = 0;
		for( int i : local )
			sum += i;
		return sum;
	}
};

static void run( iTestCase *proc, size_t length, int inserts )
{
	using namespace std::chrono;
	using stopwatch = high_resolution_clock;

	proc->prepare( length );

	const stopwatch::time_point t1 = stopwatch::now();

	const int res = proc->run( inserts );

	const stopwatch::time_point t2 = stopwatch::now();
	duration<double> time_span = duration_cast<duration<double>>( t2 - t1 );

	printf( "%i\t%.9f\t%i\n", int( length ), time_span.count(), res );
}
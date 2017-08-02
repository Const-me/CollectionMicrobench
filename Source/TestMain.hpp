#pragma once
#include <chrono>
#include <stdio.h>
#include <sstream>
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

public:
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

template<class tTest>
void run( size_t length, int inserts )
{
	tTest test;
	using namespace std::chrono;
	using stopwatch = high_resolution_clock;

	test.prepare( length );

	const stopwatch::time_point t1 = stopwatch::now();

	const int res = test.run( inserts );

	const stopwatch::time_point t2 = stopwatch::now();
	duration<double> time_span = duration_cast<duration<double>>( t2 - t1 );

	printf( "%i\t%.9f\t%i\n", int( length ), time_span.count(), res );
}

template<class tTest>
int runAll()
{
	size_t lengths[] = { 100, 1000, 100000, 1000000, 10000000, 100000000 };
	const int inserts = 5;
	for( size_t l : lengths )
		run<tTest>( l, inserts );
	return 0;
}

inline bool tryParse( const char* str, int& res )
{
	// https://stackoverflow.com/a/14176593/126995
	errno = 0;
	char *temp;
	long val = strtol( str, &temp, 0 );

	if( temp == str || *temp != '\0' || ( ( val == LONG_MIN || val == LONG_MAX ) && errno == ERANGE ) )
		return false;
	res = (int)val;
	return true;
}

inline int parseArguments( int argc, const char* argv[] )
{
	if( 2 != argc )
	{
		printf( "Usage: %s N\n", argv[ 0 ] );
		return -1;
	}
	int res;
	if( !tryParse( argv[ 1 ], res ) || res < 0 )
	{
		printf( "The argument must be a non-negative integer\n" );
		return -1;
	}
	return res;
}

#define TEST_CASE1( nTest, tType ) case nTest: printf( "%s\n", #tType ); return runAll<tType>()
#define TEST_CASE2( nTest, tType1, tType2 ) case nTest: printf( "%s, %s\n", #tType1, #tType2 ); return runAll<tType1, tType2>()
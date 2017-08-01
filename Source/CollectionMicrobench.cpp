#include "stdafx.h"
#include "PlexAlloc/Allocator.hpp"

template <typename T>
using PlexAllocator = PlexAlloc::Allocator<T>;

__interface iTestCase
{
	void prepare( size_t length );
	int run( int inserts );
};

// A test case for STL containers. Because they expose very similar API, a single template implementation is required.
template<class tContainer>
class testStl: public iTestCase
{
	tContainer local;

	void prepare( size_t length )
	{
		for( size_t i = 0; i < length; i++ )
			local.push_back( 1 );
	}
	int run( int localInserts )
	{
		for( int i = 0; i < localInserts; i++ )
			local.insert( local.begin(), 1 );

		int sum = 0;
		for( int i : local )
			sum += i;
		return sum;
	}
};

// ATL containers need special care in terms of API.
class testAtlArray: public iTestCase
{
	CAtlArray<int> local;

	void prepare( size_t length )
	{
		for( size_t i = 0; i < length; i++ )
			local.Add( 1 );
	}
	int run( int localInserts )
	{
		for( int i = 0; i < localInserts; i++ )
			local.InsertAt( 0, 1 );

		int sum = 0;
		for( size_t i = 0; i < local.GetCount(); i++ )
			sum += local[ i ];
		return sum;
	}
};

class testAtlList: public iTestCase
{
	CAtlList<int> local;

	void prepare( size_t length )
	{
		for( size_t i = 0; i < length; i++ )
			local.AddTail( 1 );
	}
	int run( int localInserts )
	{
		for( int i = 0; i < localInserts; i++ )
			local.AddHead( 1 );

		int sum = 0;
		for( POSITION pos = local.GetHeadPosition(); nullptr != pos; )
			sum += local.GetNext( pos );
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

int main()
{
	const int inserts = 5;

	size_t lengths[] = { 100, 1000, 100000, 1000000, 10000000, 100000000 };

	using tTest =
		// testStl<std::vector<int>>;
		// testStl<std::list<int>>;
		// testAtlArray;
		testAtlList;
		// testStl<std::list<int, PlexAllocator<int>>>;

	for( size_t l : lengths )
	{
		tTest coll;
		run( &coll, l, inserts );
	}
	return 0;
}
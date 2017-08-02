#include "stdafx.h"
#include "TestMain.hpp"

// ATL containers need special care in terms of API.
class testAtlArray: public iTestCase
{
	CAtlArray<int> local;

public:
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

public:
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

int main( int argc, const char* argv[] )
{
	const int nTest = parseArguments( argc, argv );
	if( nTest < 0 )
		return nTest;

	switch( nTest )
	{
		TEST_CASE1( 0, testStl<std::vector<int>> );
		TEST_CASE1( 1, testStl<std::list<int>> );
		TEST_CASE2( 2, testStl<std::list<int, PlexAllocator<int>>> );
		TEST_CASE1( 3, testAtlArray );
		TEST_CASE1( 4, testAtlList );
	};
	printf( "The argument is too large" );
	return -1;
}
#include "stdafx.h"
#include "TestMain.hpp"

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

int main()
{
	const int inserts = 5;

	size_t lengths[] = { 100, 1000, 100000, 1000000, 10000000, 100000000 };

	using tTest =
		// testStl<std::vector<int>>;
		testStl<std::list<int>>;
		// testAtlArray;
		// testAtlList;
		// testStl<std::list<int, PlexAllocator<int>>>;

	for( size_t l : lengths )
	{
		tTest coll;
		run( &coll, l, inserts );
	}
	return 0;
}
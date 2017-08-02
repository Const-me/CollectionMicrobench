#include "stdafx.h"
#include "TestMain.hpp"

// ATL containers need special care in terms of API.
class testAtlArray: public iTestCase
{
	CAtlArray<int> local;

public:
	void prepare( size_t length ) override
	{
		for( size_t i = 0; i < length; i++ )
			local.Add( 1 );
	}
	int run( int localInserts ) override
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
	void prepare( size_t length ) override
	{
		for( size_t i = 0; i < length; i++ )
			local.AddTail( 1 );
	}
	int run( int localInserts ) override
	{
		for( int i = 0; i < localInserts; i++ )
			local.AddHead( 1 );

		int sum = 0;
		for( POSITION pos = local.GetHeadPosition(); nullptr != pos; )
			sum += local.GetNext( pos );
		return sum;
	}
};

template<class tContainer>
class testAtlMap: public iTestCase
{
	tContainer local;

	template< typename K, typename V, class KTraits, class VTraits>
	inline static POSITION getStartPos( CRBMap<K, V, KTraits, VTraits>& map )
	{
		return map.GetHeadPosition();
	}

	template< typename K, typename V, class KTraits, class VTraits>
	inline static POSITION getStartPos( CAtlMap<K, V, KTraits, VTraits>& map )
	{
		return map.GetStartPosition();
	}

public:
	void prepare( size_t length ) override
	{
		std::mt19937 rr;
		rr.seed( 0 );
		for( size_t i = 0; i < length; i++ )
			local.SetAt( (int)rr(), 1 );
	}
	int run( int localInserts )
	{
		int sum = 0;
		for( POSITION pos = getStartPos( local ); nullptr != pos; )
			sum += local.GetNextValue( pos );
		return sum;
	}
	static bool isMap() { return true; }
};

int main( int argc, const char* argv[] )
{
	const int nTest = parseArguments( argc, argv );
	if( nTest < 0 )
		return nTest;

	using namespace std;
	using StdMap = map<int, int>;
	using StdMapPlex = map<int, int, less<int>, PlexAllocator<pair<const int, int>>>;
	using StdHash = unordered_map<int, int>;
	using StdHashPlex = unordered_map<int, int, hash<int>, equal_to<int>, PlexAllocator<pair<const int, int>>>;

	using AtlMap = CRBMap<int, int>;
	using AtlHash = CAtlMap<int, int>;

	switch( nTest )
	{
		TEST_CASE1( 0, testStl<std::vector<int>> );
		TEST_CASE1( 1, testStl<std::list<int>> );
		TEST_CASE2( 2, testStl<std::list<int, PlexAllocator<int>>> );
		TEST_CASE1( 3, testAtlArray );
		TEST_CASE1( 4, testAtlList );

		TEST_CASE1( 5, testStdMap<StdMap> );
		TEST_CASE1( 6, testStdMap<StdMapPlex> );
		TEST_CASE1( 7, testAtlMap<AtlMap> );
		TEST_CASE1( 8, testStdMap<StdHash> );
		TEST_CASE1( 9, testStdMap<StdHashPlex> );
		TEST_CASE1( 10, testAtlMap<AtlHash> );
	};
	printf( "The argument is too large" );
	return -1;
}
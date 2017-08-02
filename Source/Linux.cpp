#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include "TestMain.hpp"

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

	switch( nTest )
	{
		TEST_CASE1( 0, testStl<std::vector<int>> );
		TEST_CASE1( 1, testStl<std::list<int>> );
		TEST_CASE2( 2, testStl<std::list<int, PlexAllocator<int>>> );

		TEST_CASE1( 3, testStdMap<StdMap> );
		TEST_CASE1( 4, testStdMap<StdMapPlex> );
		TEST_CASE1( 5, testStdMap<StdHash> );
		TEST_CASE1( 6, testStdMap<StdHashPlex> );
	};
	printf( "The argument is too large" );
	return -1;
}
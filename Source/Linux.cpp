#include <vector>
#include <list>
#include "TestMain.hpp"

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
	};
	printf( "The argument is too large" );
	return -1;
}
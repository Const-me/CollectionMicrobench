#include <vector>
#include <list>
#include "TestMain.hpp"

int main()
{
	const int inserts = 5;

	size_t lengths[] = { 100, 1000, 100000, 1000000, 10000000, 100000000 };

	using tTest =
		// testStl<std::vector<int>>;
		testStl<std::list<int>>;
		// testStl<std::list<int, PlexAllocator<int>>>;

	for( size_t l : lengths )
	{
		tTest coll;
		run( &coll, l, inserts );
	}
	return 0;
}
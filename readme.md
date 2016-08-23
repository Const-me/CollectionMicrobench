# A C++ collections benchmark #

[Inspired by this.](https://jackmott.github.io/programming/2016/08/20/when-bigo-foolsya.html)

This benchmark tests performance of collection insert and iterate operations. Very limited scenario, I only ran each test once, used an old Windows laptop. Don’t believe the results, test with your use cases, and on your hardware.

Four test cases are implemented here:

1. `std::vector<int>`
1. `std::list<int>`
1. `CAtlArray<int>`
1. `CAtlList<int>`


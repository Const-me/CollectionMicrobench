CXX=clang++-4.0
FLAGS=-std=c++1z -stdlib=libc++ -target x86_64-pc-linux-eabi -O3 -msse2
all:
	$(CXX) $(FLAGS) Linux.cpp -o test.bin

clean:
	rm -fr build/*

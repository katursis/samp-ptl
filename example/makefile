OUTFILE = "sampptl_example.so"

COMPILE_FLAGS=-m32 -c -O3 -w
LINK_FLAGS=-m32 -shared -O3 -static-libgcc -static-libstdc++

all:
	g++ $(COMPILE_FLAGS) -std=c++17 *.cc
	g++ $(LINK_FLAGS) -o $(OUTFILE) *.o
	rm *.o

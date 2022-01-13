# The following just specifies some variables for "flexibility".

# Specify the C++ compiler
CXX     = g++

# Specify options to pass to the compiler. Here  sets the optimisation
# level, outputs debugging info for gdb, and C++ version to use.
CXXFLAGS = -O0 -g3 -std=c++14

main: main.cpp Market.o
	$(CXX) $(CXXFLAGS) main.cpp Market.o -o main

# The -c command produces the object file
Market.o: Market.cpp Market.h
	$(CXX) $(CXXFLAGS) -c Market.cpp -o Market.o

# Some cleanup functions, invoked by typing "make clean" or "make deepclean"
deepclean:
	rm -f *~ *.o *.gch Market main main.exe executions.txt *.stackdump

clean:
	rm -f *~ *.o *.stackdump

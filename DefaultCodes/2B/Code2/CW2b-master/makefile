CXX = g++

CXXFLAGS = -O0 -g3 -std=c++14

main: main.cpp Market.o
	$(CXX) $(CXXFLAGS) main.cpp Market.o -o main

Market.o: Market.cpp Market.h
	$(CXX) $(CXXFLAGS) -c Market.cpp -o Market.o

deepclean:
	rm -f *~ *.o *.gch Market main main.exe executions.txt *.stackdump

clean:
	rm -f *~ *.o *.gch *.stackdump

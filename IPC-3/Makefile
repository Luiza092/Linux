CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LDFLAGS = -pthread

first: first.o shared_array.o
	$(CXX) first.o shared_array.o -o first $(LDFLAGS)

second: second.o shared_array.o
	$(CXX) second.o shared_array.o -o second $(LDFLAGS)

first.o: first.cpp shared_array.h
	$(CXX) $(CXXFLAGS) -c first.cpp

second.o: second.cpp shared_array.h
	$(CXX) $(CXXFLAGS) -c second.cpp

shared_array.o: shared_array.cpp shared_array.h
	$(CXX) $(CXXFLAGS) -c shared_array.cpp

clean:
	rm -f *.o first second


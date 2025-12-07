CXX = g++
CXXFLAGS = -Wall -O2

TARGET = prime-calculator

all: $(TARGET)

$(TARGET): prime-calculator.o
	$(CXX) prime-calculator.o -o $(TARGET)

prime-calculator.o: prime-calculator.cpp
	$(CXX) $(CXXFLAGS) -c prime-calculator.cpp -o prime-calculator.o

clean:
	rm -f prime-calculator.o $(TARGET)

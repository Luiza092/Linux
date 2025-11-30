CXX = g++
CXXFLAGS = -Wall -O2

TARGET = signal-echo

all: $(TARGET)

$(TARGET): signal-echo.o
	$(CXX) signal-echo.o -o $(TARGET)

signal-echo.o: signal-echo.cpp
	$(CXX) $(CXXFLAGS) -c signal-echo.cpp -o signal-echo.o

clean:
	rm -f signal-echo.o $(TARGET)

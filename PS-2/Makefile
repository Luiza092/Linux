CXX = g++
CXXFLAGS = -Wall -std=c++17

TARGET = interactive-shell

all: $(TARGET)

$(TARGET): interactive-shell.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) interactive-shell.cpp

clean:
	rm -f $(TARGET) *.log 

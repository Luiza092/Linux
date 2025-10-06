CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

TARGET = copy

all: $(TARGET)

$(TARGET): source-file.cpp
	$(CXX) $(CXXFLAGS) source-file.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)

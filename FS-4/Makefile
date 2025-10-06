CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

TARGET = redirect-cin

all: $(TARGET)

$(TARGET): redirect-cin.cpp
	$(CXX) $(CXXFLAGS) redirect-cin.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)

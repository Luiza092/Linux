CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread -fPIC
LDFLAGS = -shared

LIB_NAME = libparallel_scheduler.so
LIB_SRC = parallel_scheduler.cpp
LIB_HEADER = parallel_scheduler.h
DEMO = demo-application
DEMO_SRC = main.cpp

.PHONY: all clean

all: $(LIB_NAME) $(DEMO)

$(LIB_NAME): $(LIB_SRC) $(LIB_HEADER)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LIB_SRC) -o $(LIB_NAME)

$(DEMO): $(DEMO_SRC) $(LIB_NAME)
	$(CXX) $(CXXFLAGS) $(DEMO_SRC) -L. -lparallel_scheduler -Wl,-rpath,'$$ORIGIN' -o $(DEMO)

clean:
	rm -f $(LIB_NAME) $(DEMO) *.o


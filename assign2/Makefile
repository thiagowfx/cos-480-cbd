CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -O2 -std=c++11
SOURCES=$(wildcard *.cpp) BPlusTree/bpt.cc
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=db

all: $(EXECUTABLE)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)

.PHONY: clean all

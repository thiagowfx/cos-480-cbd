CXXFLAGS = -Wall -O2 -std=c++11
SOURCES=db.cpp schema.cpp schemadb.cpp
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
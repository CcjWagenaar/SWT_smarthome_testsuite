# Compiler
CXX = g++

# Output executable name
TARGET = smarthome_tests

# Source files
SOURCES = smarthome_software.cpp smarthome_tests.cpp

# Object files (corresponding to each source file)
OBJECTS = $(SOURCES:.cpp=.o)

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra

# Default rule to build the target
all: $(TARGET)

# Rule to link object files and produce the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove all object files and the executable
clean:
	rm -f $(OBJECTS) $(TARGET)

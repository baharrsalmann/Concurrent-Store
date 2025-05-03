CXX = g++
CXXFLAGS = -g -Wall

# Default target
all: testrun

# Linking step
testrun: testrun.o hw2.o
	$(CXX) $(CXXFLAGS) -o testrun testrun.o hw2.o -lpthread

# Compilation steps
testrun.o: testrun.cpp hw2.h
	$(CXX) $(CXXFLAGS) -c testrun.cpp

hw2.o: hw2.cpp hw2.h
	$(CXX) $(CXXFLAGS) -c hw2.cpp

# Clean up
clean:
	rm -f testrun.o hw2.o testrun

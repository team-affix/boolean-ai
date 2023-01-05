SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))
HEADERS=$(wildcard ../boolean-ai/*.h)

CPPFLAGS+=-g
#CPPFLAGS+=-O2
CPPFLAGS+=-std=c++20
CPPFLAGS+=-pthread
CPPFLAGS+=-I.
CPPFLAGS+=-I"../"
CPPFLAGS+=-I"../cereal/include/"

BINARY=main

all: $(BINARY)

$(BINARY): $(OBJECTS)
	# Build the final binary
	g++ $(CPPFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	# Build a single object file (the $< token means the FIRST dependency in the list. (the %.cpp file))
	g++ $(CPPFLAGS) -c -o $@ $<

clean:
	rm -rf *.o
	rm -rf $(BINARY)


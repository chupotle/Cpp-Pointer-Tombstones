all: $(patsubst %.cpp, %.out, $(wildcard *.cpp))
%.out: %.cpp Makefile
	g++ $< -o $@

all: main.cpp
	clang++ -O3 main.cpp -Wall -std=c++11 -o raytrace

clean:
	$(RM) raytrace

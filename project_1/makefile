
UTILS = utils.o
ALGOS = tridiagonal_algorithms.o
INCL = -I./include
BUILD =  build

compile:
	g++ -c src/utils.cpp $(INCL) -o utils.o
	g++ -c src/tridiagonal_algorithms.cpp $(INCL) -o tridiagonal_algorithms.o
	g++ -c exact_solution.cpp $(INCL) -o exact_solution.o
	g++ -c thomas_algorithm.cpp $(INCL) -o thomas_algorithm.o
	g++ -c special_algorithm.cpp $(INCL) -o special_algorithm.o

link:
	g++ exact_solution.o $(UTILS) -o $(BUILD)/exact_solution
	g++ thomas_algorithm.o $(UTILS) $(ALGOS) -o $(BUILD)/thomas_algorithm
	g++ special_algorithm.o $(UTILS) $(ALGOS) -o $(BUILD)/special_algorithm

clean:
	rm -f *.o

all: compile link clean

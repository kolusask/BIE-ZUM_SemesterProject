CXX = g++
CXXFLAGS = -Wall -pedantic -Wextra -g

all: build run

build: main.o individual.o population.o random.o
	${CXX} -o knapsack main.o individual.o population.o random.o ${CXXFLAGS}

main.o: main.cpp Constants.hpp
	${CXX} -c main.cpp -o main.o ${CXXFLAGS}

individual.o: Individual.cpp Individual.hpp Constants.hpp
	${CXX} -c Individual.cpp -o individual.o ${CXXFLAGS}

population.o: Population.cpp Population.hpp Constants.hpp
	${CXX} -c Population.cpp  -o population.o ${CXXFLAGS}

random.o: Random.cpp Random.hpp Constants.hpp
	${CXX} -c Random.cpp -o random.o ${CXXFLAGS}

run: build
	./knapsack

clean:
	rm -f *.o knapsack

#include "Constants.hpp"
#include "Individual.hpp"
#include "Population.hpp"

#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

struct Item {
    double value;
    double weight;
    void operator += (const Item& other) {
        this->value += other.value;
        this->weight += other.weight;
    }
};

/// Reads input from the user in the specified format (W n v[0] w[0] v[1] 2[1] ...)
std::vector<Item> read_input(double& maxWeight) {
    std::cout << "MaxWeight nItems" << std::endl;
    size_t nItems;
    std::cin >> maxWeight >> nItems;
    std::vector<Item> items(nItems);
    std::cout << "value weight" << std::endl;
    for (size_t i = 0; i < nItems; i++)
        std::cin >> items[i].value >> items[i].weight;
    return items;
}

/// Builds the fit function based on the user input
FitLambda construct_lambda(const std::vector<Item>& items, const double maxWeight) {
    auto fitLambda = [&items, maxWeight] (const Genome& genome) -> double {
        Item common = { 0, 0 };
        for (size_t i = 0; i < genome.size(); i++) {
            if (genome[i]) {
                common += items[i];
                if (common.weight > maxWeight)
                    return 0;
            }
        }
        return common.value;
    };
    return fitLambda;
}

/// Driver function for the evolution process
double evolve(const size_t genomeSize, const FitLambda& fitFun, Genome& genome) {
    Population population(genomeSize, fitFun);
    for (size_t i = 0; i < NUMBER_OF_GENERATIONS; i++)
            population = population.evolve();
    Individual best = std::move(population.find_best(fitFun));
    genome = best.genome();
    return best.fitness(fitFun);
}

/// Gets the total weight of all items selected by a given genome
double calc_weight(const Genome& genome, const std::vector<Item>& items) {
    double weight = 0;
    for (size_t i = 0; i < genome.size(); i++)
        if (genome[i])
            weight += items[i].weight;
    return weight;
}

/// Prints the algorithm output in a fancy format
void print_result(const std::vector<Item>& items, const Genome& genome) {
    std::cout << "Elements to put into a knapsack:" << std::endl;
    std::cout << "Number\tValue\tWeight" << std::endl;
    for (size_t i = 0; i < genome.size(); i++)
        if (genome[i]) 
            std::cout << i + 1 << ":\t" << items[i].value << '\t' << items[i].weight << std::endl;
}

int main() {
    double maxWeight;
    std::vector<Item> items = read_input(maxWeight);
    FitLambda fitLambda = construct_lambda(items, maxWeight);

    Genome genome;
    double valueSum = evolve(items.size(), fitLambda, genome);

    double weightSum = calc_weight(genome, items);
    if (weightSum <= maxWeight) {
        print_result(items, genome);
        std::cout << "Total value:\t" << valueSum << std::endl;
        std::cout << "Total weight:\t" << weightSum << std::endl;
    } else {
        std::cout << "Sorry, the knapsack is too small" << std::endl;
    }

    return 0;
}

/*
    20 8
    1 10
    2 10
    3 10
    4 10
    5 100
    6 100
    7 100
    8 100
*/
/*
    50 3
    60 10
    100 20
    120 30
*/

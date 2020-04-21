#pragma once

#include "Constants.hpp"
#include "Individual.hpp"

#include <algorithm>
#include <functional>
#include <vector>

using Individuals = std::vector<Individual>;
using Parents = std::pair<Individual, Individual>;

class Population {
  public:
    Population(const size_t genSize, const std::function<size_t(const Genome&)>& fitFun);
    void operator=(const Population& other);
    Population evolve() const;
    Genome find_best(const FitLambda& fitFun) const;
    size_t fitness_sum() const;

  private:
    Population(const std::function<size_t(const Genome&)>& fitFun, const Individuals& ind);
    const Individual& select_1() const;
    const Parents select_2() const;

    const std::function<size_t(const Genome&)> m_CalcFitness;
    Individuals m_Individuals;
    size_t m_FitnessSum;
};

Population::Population(const size_t genSize, const std::function<size_t(const Genome&)>& fitFun) : m_CalcFitness(fitFun), m_FitnessSum(0) {
    for (size_t i = 0; i < POPULATION_SIZE; i++) {
        Individual ind = Individual::random(genSize);
        m_Individuals.push_back(ind);
        m_FitnessSum += ind.fitness(fitFun);
    }
}

Population::Population(const std::function<size_t(const Genome&)>& fitFun, const Individuals& ind) : m_CalcFitness(fitFun), m_Individuals(std::move(ind)) {
    m_FitnessSum = std::accumulate(
        ind.begin(), ind.end(), size_t(0), 
        [&fitFun](const Individual& i1, const Individual& i2) -> size_t { 
            return i1.fitness(fitFun) + i2.fitness(fitFun); 
        }
    );
}

void Population::operator=(const Population& other) {
    this->m_Individuals = std::move(other.m_Individuals);
    this->m_FitnessSum = other.m_FitnessSum;
}

Population Population::evolve() const {
    Individuals nextPop;
    for (size_t i = 0; i < POPULATION_SIZE; i++) {
        const Parents parents = select_2();
        Children children = parents.first + parents.second;
        nextPop.push_back(children.first);
        nextPop.push_back(children.second);
    }
    return Population(m_CalcFitness, nextPop);
}

Genome Population::find_best(const FitLambda& fitFun) const {
    return std::max_element(
        m_Individuals.begin(), m_Individuals.end(),
        [&fitFun] (const Individual& i1, const Individual& i2) -> bool {
            return i1.fitness(fitFun) < i2.fitness(fitFun);
        }
    )->genome();
}

size_t Population::fitness_sum() const {
    return m_FitnessSum;
}

const Individual& Population::select_1() const {
    const size_t select = Random::generate().next_size_t(m_FitnessSum);
    size_t tempSum = 0;
    for (const auto& ind : m_Individuals) {
        tempSum += ind.fitness(m_CalcFitness);
        if (tempSum >= select)
            return ind;
    }
    throw 5;
}

const Parents Population::select_2() const {
    const Individual i1 = select_1();
    const Individual i2 = select_1();
    if (&i1 == &i2)
        return select_2();
    return Parents(i1, i2);
}

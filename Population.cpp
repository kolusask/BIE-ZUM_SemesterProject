#include "Population.hpp"

Population::Population(const size_t genSize, const FitLambda& fitFun) : m_CalcFitness(fitFun), m_FitnessSum(0) {
    for (size_t i = 0; i < POPULATION_SIZE; i++) {
        Individual ind = Individual::random(genSize);
        m_Individuals.push_back(ind);
        m_FitnessSum += ind.fitness(fitFun);
    }
}

Population::Population(const FitLambda& fitFun, const Individuals& ind) : m_CalcFitness(fitFun), m_Individuals(std::move(ind)) {
    m_FitnessSum = std::accumulate(
        m_Individuals.begin(), m_Individuals.end(), size_t(0), 
        [&fitFun](const double val, const Individual& ind) -> double { 
            return val + ind.fitness(fitFun); 
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

Individual Population::find_best(const FitLambda& fitFun) const {
    return *std::max_element(
        m_Individuals.begin(), m_Individuals.end(),
        [&fitFun] (const Individual& i1, const Individual& i2) -> bool {
            return i1.fitness(fitFun) < i2.fitness(fitFun);
        }
    );
}

const Individual& Population::select_1() const {
    const double select = Random::generate().next_double(m_FitnessSum);
    double tempSum = 0;
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

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

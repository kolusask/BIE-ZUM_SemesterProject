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
    Population(const size_t genSize, const FitLambda& fitFun);
    void operator=(const Population& other);
    Population evolve() const;
    Individual find_best(const FitLambda& fitFun) const;

  private:
    Population(const FitLambda& fitFun, const Individuals& ind);
    const Individual& select_1() const;
    const Parents select_2() const;

    const FitLambda m_CalcFitness;
    Individuals m_Individuals;
    double m_FitnessSum;
};

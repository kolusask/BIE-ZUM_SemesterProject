#pragma once

#include "Constants.hpp"
#include "Individual.hpp"

#include <algorithm>
#include <functional>
#include <vector>

using Individuals = std::vector<Individual>;
using Parents = std::pair<Individual, Individual>;

/**
 * @class
 * Represents a population of individuals in one particular generation.
 * Can perform selection of pairs of individuals giving higher probability to the fittest ones.
 * Then lets them reproduce and forms a population for the next generation out of their offsprings.
 */
class Population {
  public:
    /// Constructor of a population of randomly generated indiviuals
    Population(const size_t genSize, const FitLambda& fitFun);

    /// Copy individuals from another population
    void operator=(const Population& other);

    /// Perform selection and reproduction of (ideally) fittest individuals
    Population evolve() const;

    /// Identify the fittest individual based on the fit function
    Individual find_best(const FitLambda& fitFun) const;

  private:
    /// Constructor from fitness function and already existing individuals
    Population(const FitLambda& fitFun, const Individuals& ind);

    /// Selects one individual giving the higher probability to the fittest ones
    const Individual& select_1() const;

    /// Selects two (more likely) fittest individuals that will produce an offspring
    const Parents select_2() const;

    /// Fit function for calculating fitness of an individual
    const FitLambda m_CalcFitness;

    /// Individuals of the current population
    Individuals m_Individuals;

    /// Total fitness of all individuals (used in selection)
    double m_FitnessSum;
};

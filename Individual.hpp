#pragma once

#include "Constants.hpp"
#include "Random.hpp"

#include <functional>
#include <random>
#include <vector>

class Individual;

using Children = std::pair<Individual, Individual>;
using Genome = std::vector<bool>;
using FitLambda = std::function<double(const Genome&)>;

class Individual {
  public:
    /// Constructor from genome size
    Individual(const size_t size = s_GenomeSize);

    /// Create a Individual with random genome
    static Individual random(const size_t size);

    /// Produce a Individual from two parents (using crossover and mutation)
    Children operator+(const Individual& other) const;

    /// Calculate fitness from genome using passed function
    double fitness(const FitLambda& fitFun) const;

    /// Get genome
    Genome genome() const;

    /// Getter for genome size
    size_t genome_size() const;

  private:
    static Children crossover(const Individual& p1, const Individual& p2);
    void mutate();

    Genome m_Genome;

    mutable bool m_FitnessKnown = false;
    mutable double m_Fitness;

    static size_t s_GenomeSize;
};

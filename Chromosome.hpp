#pragma once

#include "Constants.hpp"
#include "Random.hpp"

#include <random>
#include <vector>

class Chromosome;

using Children = std::pair<Chromosome, Chromosome>;

class Chromosome {
  public:
    /// Constructor from genome size
    Chromosome(const size_t size = s_GenomeSize);

    /// Create a chromosome with random genome
    static Chromosome random(const size_t size);

    /// Produce a chromosome from two parents (using crossover and mutation)
    Children operator+(const Chromosome& other) const;

  private:
    bool operator[](const size_t i) const;
    static Children crossover(const Chromosome& p1, const Chromosome& p2);
    void mutate();
    
    std::vector<bool> m_Genome;

    static size_t s_GenomeSize;
};

size_t Chromosome::s_GenomeSize = 0;

Chromosome::Chromosome(const size_t size) {
    m_Genome.reserve(size);
    s_GenomeSize = size;
}

Chromosome Chromosome::random(const size_t size) {
    Chromosome result(size);
    for (size_t i = 0; i < size; i++)
        result.m_Genome.push_back(Random::generate().next_bool());
    return std::move(result);
}

Children Chromosome::operator+(const Chromosome& other) const {
    Children children = crossover(*this, other);
    children.first.mutate();
    children.second.mutate();
    return std::move(children);
}

bool Chromosome::operator[](const size_t i) const { return this->m_Genome[i]; }

Children Chromosome::crossover(const Chromosome& p1, const Chromosome& p2) {
#if CROSSOVER_POINTS > 0    
    if (Random::generate().next_bool(CROSSOVER_PROB)) {
        size_t points[CROSSOVER_POINTS];
        for (size_t i = 0; i < CROSSOVER_POINTS; i++)
            points[i] = Random::generate().next_size_t(CROSSOVER_POINTS);
        Children children;
        for (size_t i = 0; i < CROSSOVER_POINTS - 1; i++) {
            const size_t start = points[i];
            const size_t stop = points[i + 1];
            const Chromosome& parent = i % 2 ? p2 : p1;
            for (size_t j = start; j < stop; j++) {
                children.first.m_Genome.push_back(parent[i]);
                children.second.m_Genome.push_back(!parent[i]);
            }
        }
#if CROSSOVER_POINTS % 2 == 0
        const Chromosome& parent = p1;
#else
        const Chromosome& parent = p2;
#endif
        for (size_t i = points[CROSSOVER_POINTS - 1]; i < s_GenomeSize; i++) {
            children.first.m_Genome.push_back(parent[i]);
            children.second.m_Genome.push_back(!parent[i]);
        }
    }   
#endif
    return Children(p1, p2);
}

void Chromosome::mutate() {
    for (size_t i = 0; i < s_GenomeSize; i++)
        if (Random::generate().next_bool(MUTATION_PROB))
            m_Genome[i] = !m_Genome[i];
}

#include "Individual.hpp"

size_t Individual::s_GenomeSize = 0;

Individual::Individual(const size_t size) : m_Genome(size) {
    s_GenomeSize = size;
}

Individual Individual::random(const size_t size) {
    Individual result(size);
    for (size_t i = 0; i < size; i++)
        result.m_Genome[i] = (Random::generate().next_bool());
    return result;
}

Children Individual::operator+(const Individual& other) const {
    Children children = crossover(*this, other);
    children.first.mutate();
    children.second.mutate();
    return children;
}

double Individual::fitness(const FitLambda& fitFun) const {
    if (!m_FitnessKnown)
        m_Fitness = fitFun(m_Genome);
    return m_Fitness;
}

Genome Individual::genome() const {
    return m_Genome;
}

size_t Individual::genome_size() const {
    return s_GenomeSize;
}

Children Individual::crossover(const Individual& p1, const Individual& p2) {
#if CROSSOVER_POINTS > 0    
    if (Random::generate().next_bool(CROSSOVER_PROB)) {
        size_t points[CROSSOVER_POINTS];
        for (size_t i = 0; i < CROSSOVER_POINTS; i++)
            points[i] = Random::generate().next_size_t(CROSSOVER_POINTS);
        Children children;
        for (size_t i = 0; i < CROSSOVER_POINTS - 1; i++) {
            const size_t start = points[i];
            const size_t stop = points[i + 1];
            const Individual& parent = i % 2 ? p2 : p1;
            for (size_t j = start; j < stop; j++) {
                children.first.m_Genome[j] = parent.m_Genome[j];
                children.second.m_Genome[j] = !parent.m_Genome[j];
            }
        }
#if CROSSOVER_POINTS % 2 == 0
        const Individual& parent = p1;
#else
        const Individual& parent = p2;
#endif
        for (size_t i = points[CROSSOVER_POINTS - 1]; i < s_GenomeSize; i++) {
            children.first.m_Genome[i] = parent.m_Genome[i];
            children.second.m_Genome[i] = !parent.m_Genome[i];
        }
        return children;
    }   
#endif
    return Children(p1, p2);
}

void Individual::mutate() {
    for (size_t i = 0; i < s_GenomeSize; i++)
        if (Random::generate().next_bool(MUTATION_PROB))
            m_Genome[i] = !m_Genome[i];
}

#include "Random.hpp"

std::unique_ptr<Random> Random::s_Instance = nullptr;
std::random_device Random::s_Rd;
std::mt19937 Random::s_Gen(s_Rd());

Random& Random::generate() {
    if (!s_Instance) {
        struct Enabler : public Random { using Random::Random; };
        s_Instance = std::make_unique<Enabler>();
    }
    return *s_Instance;
}

bool Random::next_bool(const double prob) const {
    return next_double() < prob;
}

size_t Random::next_size_t(const size_t max) const {
    std::uniform_int_distribution<size_t> uid(0, max - 1);
    return uid(s_Gen);
}

double Random::next_double(const double max) const {
    std::uniform_real_distribution<double> urd(0, max);
    return urd(s_Gen);
}

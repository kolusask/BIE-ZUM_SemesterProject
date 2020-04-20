#pragma once

#include <memory>
#include <random>

class Random {
  public:
    static Random& generate();
    bool next_bool(const double prob = 0.5) const;
    size_t next_size_t(const size_t max) const;
    double next_double(const double max = 1) const;
    
  private:
    Random();

    static std::shared_ptr<Random> s_Instance;
    static std::random_device s_Rd;  //Will be used to obtain a seed for the random number engine
    static std::mt19937 s_Gen;

};

std::shared_ptr<Random> Random::s_Instance = nullptr;
std::random_device Random::s_Rd;
std::mt19937 Random::s_Gen(s_Rd);

Random& Random::generate() {
    if (!s_Instance)
        s_Instance = std::make_shared<Random>();
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

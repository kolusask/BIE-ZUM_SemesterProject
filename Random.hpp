#pragma once

#include <memory>
#include <random>

class Random {
  public:
    static Random& generate();
    bool next_bool(const double prob = 0.5) const;
    size_t next_size_t(const size_t max) const;
    double next_double(const double max = 1) const;
    
  protected:
    static std::unique_ptr<Random> s_Instance;
    static std::random_device s_Rd;  //Will be used to obtain a seed for the random number engine
    static std::mt19937 s_Gen;

};

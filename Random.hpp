#pragma once

#include <memory>
#include <random>

/**
 * @class
 * A supplementary singleton class for generating random values using a generator from the Standard library.
 */
class Random {
  public:
    /// Returns the only instance of the class
    static Random& generate();

    /// Generates a boolean value whose probability of being true is \a prob
    bool next_bool(const double prob = 0.5) const;

    /// Generates a size_t value in the range from 0 to \a max
    size_t next_size_t(const size_t max) const;

    /// Generates a double value in the range from 0 to \a max
    double next_double(const double max = 1) const;
    
  protected:
    static std::unique_ptr<Random> s_Instance;
    static std::random_device s_Rd;  //Will be used to obtain a seed for the random number engine
    static std::mt19937 s_Gen;

};

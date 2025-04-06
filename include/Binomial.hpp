#include <ctime>
#include <random>

std::default_random_engine      engine(static_cast<unsigned int>(time(0)));
std::binomial_distribution<int> binom(2, 0.5); // 2 trials, 100% chance each
bool                            didBinomial    = false;
float                           binomialResult = .0f;

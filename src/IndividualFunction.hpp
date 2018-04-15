#ifndef INDIVIDUAL_FUNCTION_HPP
#define INDIVIDUAL_FUNCTION_HPP

#include <vector>
#include <iterator>
#include <random>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Individual.hpp"

class IndividualFunction : public Individual
{
  private:
    double (*objetiveFunction)(std::vector<double> x);
    double minDomainValue;
    double maxDomainValue;
    double step;
    size_t bits;
    size_t dimensions;

    std::vector<bool> genotype;

    std::mt19937 gen;
    std::uniform_real_distribution<> randProbability;
    std::uniform_int_distribution<> randGen;

  public:
    IndividualFunction() = default;
    IndividualFunction(double (*objetiveFunction)(std::vector<double>), double minDomainValue, double maxDomainValue, size_t bits, size_t dimensions);

    void initRandom();
    void mutate(const double probability);
    void cross(const Individual &partner, const size_t pos);

    void setFitness();
    const std::vector<bool> &getGenotype() const;
    std::vector<double> getFenotype();

    bool toFile(const char* filename);

    bool operator<(const IndividualFunction &ind) const;
};

#endif
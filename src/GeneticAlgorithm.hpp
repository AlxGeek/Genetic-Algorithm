#ifndef GENETIC_ALGORITHM_HPP
#define GENETIC_ALGORITHM_HPP

#include <vector>
#include <memory>
#include <cstddef>
#include <random>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <set>
#include <type_traits>

//#include "Individual.hpp"

template<class T>
class GeneticAlgorithm
{
private:
  size_t genotypeLength;
  double mutationProbability;
  double crossoverProbability;
  size_t eliteNumber;

  size_t populationSize;
  std::vector<T> population;
  std::vector<T> offspring;

  std::random_device rd;
  std::mt19937 gen;
  std::uniform_int_distribution<> randPopulation;
  std::uniform_int_distribution<> randGenotype;
  std::uniform_real_distribution<> randProb;

  void tournament(size_t n);
  void crossover();
  void mutation();
  void calcFitness();
  void elitism();
  void multiDynamic(double D);

  std::vector<size_t> nonDominated(std::vector<T> individuals);

public:
  GeneticAlgorithm(const T &individual, size_t populationSize, double mutationProbability, double crossoverProbability, size_t eliteNumber);
  void initPoblation();
  int run(int maxSeconds);
  const T &getBest();
};

#include "GeneticAlgorithm.tpp"

#endif //GENETIC_ALGORITHM_HPP
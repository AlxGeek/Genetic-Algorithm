#ifndef SUDOKU_HPP
#define SUDOKU_HPP

#include <bitset>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>

#include <cstring>

#include <iostream>

#include "Individual.hpp"

class Sudoku : public Individual
{

private:
  size_t weightOriginalConflict = 20;
  size_t step;
  size_t sudokuSize;
  std::vector<std::vector<unsigned short>> solution;
  std::vector<std::vector<unsigned short>> original;
  std::vector<std::vector<int>> conflictsTable;
  std::vector<std::vector<int>> conflictsPermutationTable;
  std::vector<std::vector<unsigned short>> tableFreeCells;
  std::vector<std::vector<std::vector<unsigned short>>> permutationsPerBlock;
  std::vector<std::vector<unsigned short>> *permutationsBlock;
  std::vector<std::vector<unsigned short>> missingNumbersTable;

  std::mt19937 gen;
  std::uniform_real_distribution<> randProbability;

  void print(std::vector<std::vector<unsigned short>> &board);
  int getConflicts(std::vector<int> &hist);
  int getConflicts(unsigned short value, size_t k, size_t l);
  void setPermutationsPerBlock();
  void initMissingNumbersTable();
  void setFreeCells();
  void setSquare(std::vector<unsigned short> &values, size_t k, size_t l);
  void createConstructiveHeuristicSquare(int k, int l);
  std::vector<unsigned short> getMissingElementsSquare(size_t k, size_t l);
  std::vector<unsigned short> getSuffledMissingElementsSquare(size_t k, size_t l);
  bool stochasticLocalSearchSquare(int k, int l, size_t step);
  bool stochasticLocalSearchAllSquare(size_t k, size_t l);
  void createRandomSquare(int k, int l);
  bool readFromFile(std::string filename);

public:
  Sudoku() = default;
  Sudoku(std::string filename);

  void createSolution(bool useRandom);
  void createRandomSolution();
  void createConstructiveHeuristicSolution();

  size_t stochasticLocalSearch(size_t repetitions);
  size_t stochasticLocalSearchAll(size_t repetitions);

  void printOriginal();
  void printSolution();

  const std::vector<std::vector<unsigned short>> &getSolution() const;

  int getConflicts();
  int getConflictsRows();
  int getConflictsCols();
  int getConflictsRowsAndCols();
  int getConflictsSquare();

  void simulatedAnnealing(double t, double tMin);

  void initRandom();
  void setFitness();
  void mutate(double probability);
  void cross(const Individual &individual, const size_t pos);
  size_t getGenotypeLength() const;
  void setDCN(const std::vector<Sudoku> &survivors);
  double getDistance(const Sudoku &sud);
};

#endif //SUDOKU_HPP
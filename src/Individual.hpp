#ifndef INDIVIDUAL_HPP
#define INDIVIDUAL_HPP

#include <cstdlib>
#include <vector>

class Individual
{
private:
protected:
  double fitness;
  double dcn;

public:
  double getFitness() const;

  virtual void initRandom() = 0;

  virtual void setFitness() = 0;

  virtual void mutate(double probability) = 0;

  virtual void cross(const Individual &partner, const size_t pos) = 0;

  double getDCN() const;

  void setDCN(const std::vector<Individual> &survivors);

  virtual size_t getGenotypeLength() const = 0;

  bool operator<(const Individual &ind) const;

  void stochasticLocalSearch(int n);
};

#endif // INDIVIDUAL_HPP
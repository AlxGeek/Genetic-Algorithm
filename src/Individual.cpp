#include "Individual.hpp"

double Individual::getFitness() const
{
    return fitness;
}

double Individual::getDCN() const
{
    return dcn;
}

bool Individual::operator<(const Individual &ind) const
{
    return fitness < ind.getFitness();
}
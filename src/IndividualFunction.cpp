#include "IndividualFunction.hpp"

IndividualFunction::IndividualFunction(
    double (*objetiveFunction)(std::vector<double>), double minDomainValue, double maxDomainValue, size_t bits, size_t dimensions)
    : objetiveFunction(objetiveFunction), minDomainValue(minDomainValue), maxDomainValue(maxDomainValue),
      step((maxDomainValue - minDomainValue) / (pow(2, bits) - 1)),
      bits(bits), dimensions(dimensions), genotype(bits * dimensions),
      randProbability(0, 100), randGen(0, 1)
{    
}

std::vector<double> IndividualFunction::getFenotype()
{
    std::vector<double> fenotype(dimensions, 0);
    for (size_t i = 0; i < dimensions; i++)
    {
        for (size_t j = 0; j < bits; j++)
        {
            fenotype[i] += pow(2, j) * genotype[(i * bits) + j];
        }
        fenotype[i] = minDomainValue + fenotype[i] * step;
    }
    return fenotype;
}

bool IndividualFunction::toFile(const char *filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        return false;
    }
    file << getFitness() << std::endl;
    for (size_t i = 0; i < genotype.size(); i++)
    {
        file << genotype[i] << " ";
    }
    file << std::endl;
    std::vector<double> fenotype = getFenotype();
    for (auto f : fenotype)
    {
        file << f << " ";
    }
    file << std::endl;
    file.close();
    return true;
}

void IndividualFunction::initRandom()
{
    std::random_device rd1;
    gen.seed(rd1());
    for (size_t i = 0; i < genotype.size(); i++)
    {
        genotype[i] = randGen(gen);
    }
    setFitness();
}

void IndividualFunction::mutate(const double probability)
{
    for (size_t i = 0; i < genotype.size(); i++)
    {
        if (randProbability(gen) < probability)
        {
            genotype[i] = !genotype[i];
        }
    }
}

void IndividualFunction::cross(const Individual &partner, const size_t pos)
{
    const IndividualFunction &p = static_cast<const IndividualFunction &>(partner);
    std::copy(std::next(p.getGenotype().begin(), pos), p.getGenotype().end(), std::next(genotype.begin(), pos));
}

void IndividualFunction::setFitness()
{
    fitness = objetiveFunction(getFenotype());
}

const std::vector<bool> &IndividualFunction::getGenotype() const
{
    return genotype;
}

bool IndividualFunction::operator<(const IndividualFunction &ind) const
{
    return fitness < ind.getFitness();
}
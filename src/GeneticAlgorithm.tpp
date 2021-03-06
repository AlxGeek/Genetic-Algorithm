#include "GeneticAlgorithm.hpp"

template<class T>
GeneticAlgorithm<T>::GeneticAlgorithm(
    const T &individual, size_t populationSize, double mutationProbability, double crossoverProbability, size_t eliteNumber)
    : genotypeLength(individual.getGenotypeLength()), mutationProbability(mutationProbability),
      crossoverProbability(crossoverProbability), eliteNumber(eliteNumber),
      populationSize(populationSize), population(populationSize, individual), offspring(populationSize),
      rd(), gen(rd()), randPopulation(0, populationSize - 1), randGenotype(0, genotypeLength - 1), randProb(0, 100)
{
}

template<class T>
void GeneticAlgorithm<T>::initPoblation()
{
    for (auto &i : population)
    {
        i.initRandom();
    }
}

template<class T>
void GeneticAlgorithm<T>::tournament(size_t n)
{
    for (size_t i = eliteNumber; i < populationSize; i++)
    {
        size_t selected = randPopulation(gen);
        double min = population[selected].getFitness();
        for (size_t j = 0; j < n - 1; j++)
        {
            size_t k = randPopulation(gen);
            if (population[k].getFitness() < min)
            {
                selected = k;
                min = population[k].getFitness();
            }
        }
        offspring[i] = population[selected];
    }
}

template<class T>
void GeneticAlgorithm<T>::crossover()
{
    T *aux;
    for (size_t i = eliteNumber; i < populationSize - 1; i += 2)
    {
        if (randProb(gen) < crossoverProbability)
        {
            aux = &offspring[i];
            size_t pos = randGenotype(gen);
            offspring[i].cross(offspring[i + 1], pos);
            offspring[i + 1].cross(*aux, pos);
        }
    }
}

template<class T>
void GeneticAlgorithm<T>::mutation()
{
    for (size_t i = eliteNumber; i < populationSize; i++)
    {
        offspring[i].mutate(mutationProbability);
    }
}

template<class T>
void GeneticAlgorithm<T>::calcFitness()
{
    for (size_t i = eliteNumber; i < populationSize; i++)
    {
        offspring[i].setFitness();
    }
}

template<class T>
void GeneticAlgorithm<T>::elitism()
{
    std::sort(population.begin(), population.end());
    for (size_t i = 0; i < eliteNumber; i++)
    {
        offspring[i] = population[i];
    }
}

template<class T>
void GeneticAlgorithm<T>::multiDynamic(double D)
{
    size_t i;
    size_t c;
    std::vector<size_t> nd;
    std::vector<T> newPop;
    newPop.reserve(populationSize);
    std::vector<T> currentMembers = population;
    currentMembers.reserve(2 * populationSize);
    currentMembers.insert(currentMembers.end(), offspring.begin(), offspring.end());
    typename std::vector<T>::iterator best = std::min_element(currentMembers.begin(), currentMembers.end());
    newPop.push_back(*best);
    currentMembers.erase(best);
    while (newPop.size() < populationSize)
    {
        for (auto &cm : currentMembers)
        {
            cm.setDCN(newPop);
        }
        nd = nonDominated(currentMembers);
        c = 0;
        do
        {
            i = rand() % nd.size();
            c++;
        } while (currentMembers[nd[i]].getDCN() < D && c < nd.size());
        newPop.push_back(currentMembers[nd[i]]);
        currentMembers.erase(currentMembers.begin() + nd[i]);
    }
    population = newPop;
}

template<class T>
std::vector<size_t> GeneticAlgorithm<T>::nonDominated(std::vector<T> individuals)
{
    std::vector<size_t> nd;
    for (size_t i = 0; i < individuals.size(); i++)
    {
        bool nonDominated = true;
        for (size_t j = 0; j < individuals.size(); j++)
        {
            if (individuals[i].getFitness() > individuals[j].getFitness() && individuals[i].getDCN() < individuals[j].getDCN())
            {
                nonDominated = false;
                break;
            }
        }
        if (nonDominated)
        {
            nd.push_back(i);
        }
    }
    return nd;
}

template<class T>
int GeneticAlgorithm<T>::run(int maxSeconds)
{
    double DI = 10;
    int i = 0;
    auto start = std::chrono::steady_clock::now();
    for (auto &p : population)
    {
        p.stochasticLocalSearch(20);
    }
    do
    {
        tournament(2);
        crossover();
        mutation();
        for (auto &o : offspring)
        {
            o.stochasticLocalSearch(20);
        }
        calcFitness();
        multiDynamic(DI - DI * (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() / maxSeconds));
        if (getBest().getFitness() == 0)
        {
            return i;
        }
        i++;
    } while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < maxSeconds);
    return i;
}

template<class T>
const T &GeneticAlgorithm<T>::getBest()
{
    return *std::min_element(population.begin(), population.end());
}
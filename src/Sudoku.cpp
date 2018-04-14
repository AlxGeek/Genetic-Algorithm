#include "Sudoku.hpp"

Sudoku::Sudoku(std::string filename)
{
    readFromFile(filename);
    step = sqrt(original.size());
    initMissingNumbersTable();
    setFreeCells();
}

void Sudoku::setPermutationsPerBlock()
{
    permutationsPerBlock.resize(original.size());
    for (size_t i = 0; i < permutationsPerBlock.size(); i++)
    {
        std::vector<unsigned short> options = missingNumbersTable[i];
        do
        {
            permutationsPerBlock[i].push_back(options);
        } while (std::next_permutation(options.begin(), options.end()));
    }
}

void Sudoku::initMissingNumbersTable()
{
    missingNumbersTable.resize(original.size());
    for (size_t k = 0; k < original.size(); k += step)
    {
        for (size_t l = 0; l < original.size(); l += step)
        {
            missingNumbersTable[l / step + k] = getMissingElementsSquare(k, l);
        }
    }
}

void Sudoku::setFreeCells()
{
    tableFreeCells.resize(original.size());
    for (size_t k = 0; k < original.size(); k += step)
    {
        for (size_t l = 0; l < original.size(); l += step)
        {
            unsigned short cell = 0;
            for (size_t i = k; i < k + step; i++)
            {
                for (size_t j = l; j < l + step; j++, cell++)
                {
                    if (original[i][j] == 0)
                    {
                        tableFreeCells[l / step + k].push_back(cell);
                    }
                }
            }
        }
    }
}

void Sudoku::setSquare(std::vector<unsigned short> &values, size_t k, size_t l)
{
    size_t pos = 0;
    for (size_t i = k; i < k + step; i++)
    {
        for (size_t j = l; j < l + step; j++)
        {
            if (original[i][j] == 0)
            {
                solution[i][j] = values[pos];
                pos++;
            }
        }
    }
}

bool Sudoku::readFromFile(std::string filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "No se pudo abrir el archivo: " << filename << std::endl;
        return false;
    }
    std::string line;
    while (std::getline(file, line))
    {
        std::vector<unsigned short> row;
        std::istringstream iss(line);
        std::string s;
        while (std::getline(iss, s, ' '))
        {
            row.push_back(std::stoul(s));
        }
        original.push_back(row);
    }
    file.close();
    return true;
}

void Sudoku::createSolution(bool useRandom)
{
    if (useRandom)
    {
        createRandomSolution();
    }
    else
    {
        createConstructiveHeuristicSolution();
    }
}

void Sudoku::createRandomSolution()
{
    solution = original;
    for (size_t k = 0; k < solution.size(); k += step)
    {
        for (size_t l = 0; l < solution.size(); l += step)
        {
            createRandomSquare(k, l);
        }
    }
}

void Sudoku::createRandomSquare(int k, int l)
{
    std::vector<unsigned short> options = getSuffledMissingElementsSquare(k, l);
    for (size_t i = k; i < k + step; i++)
    {
        for (size_t j = l; j < l + step; j++)
        {
            if (original[i][j] == 0)
            {
                solution[i][j] = options.back();
                options.pop_back();
            }
        }
    }
}

void Sudoku::createConstructiveHeuristicSolution()
{
    solution = original;
    for (size_t k = 0; k < solution.size(); k += step)
    {
        for (size_t l = 0; l < solution.size(); l += step)
        {
            createConstructiveHeuristicSquare(k, l);
        }
    }
}

void Sudoku::createConstructiveHeuristicSquare(int k, int l)
{
    std::vector<unsigned short> options = getSuffledMissingElementsSquare(k, l);
    for (size_t i = k; i < k + step; i++)
    {
        for (size_t j = l; j < l + step; j++)
        {
            if (original[i][j] == 0)
            {
                for (auto option : options)
                {
                    if (std::find(solution[i].begin(), solution[i].end(), option) == solution[i].end())
                    {
                        solution[i][j] = option;
                        options.erase(std::find(options.begin(), options.end(), option));
                        break;
                    }
                }
                if (solution[i][j] == 0)
                {
                    solution[i][j] = options.back();
                    options.pop_back();
                }
            }
        }
    }
}

std::vector<unsigned short> Sudoku::getMissingElementsSquare(size_t k, size_t l)
{
    std::vector<unsigned short> options(original.size());
    std::iota(options.begin(), options.end(), 1);
    for (size_t i = k; i < k + step; i++)
    {
        for (size_t j = l; j < l + step; j++)
        {
            if (original[i][j] != 0)
            {
                options.erase(std::find(options.begin(), options.end(), original[i][j]));
            }
        }
    }
    return options;
}

std::vector<unsigned short> Sudoku::getSuffledMissingElementsSquare(size_t k, size_t l)
{
    std::vector<unsigned short> options = getMissingElementsSquare(k, l);
    std::random_shuffle(options.begin(), options.end());
    return options;
}

void Sudoku::print(std::vector<std::vector<unsigned short>> &board)
{
    for (auto &row : board)
    {
        for (auto i : row)
        {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Sudoku::printSolution()
{
    print(solution);
}

void Sudoku::printOriginal()
{
    print(original);
}

const std::vector<std::vector<unsigned short>> &Sudoku::getSolution() const
{
    return solution;
}

int Sudoku::getConflicts()
{
    return getConflictsRowsAndCols();
}

int Sudoku::getConflicts(std::vector<int> &hist)
{
    int conflicts = 0;
    for (auto i : hist)
    {
        conflicts += i == -1 ? 1 : i;
    }
    return conflicts;
}

int Sudoku::getConflicts(unsigned short value, size_t k, size_t l)
{
    int conflicts = 0;
    for (size_t i = 0; i < solution.size(); i++)
    {
        if (l != i)
        {
            if (value == original[k][i])
            {
                conflicts += weightOriginalConflict;
            }
            else if (value == solution[k][i])
            {
                conflicts++;
            }
        }
        if (k != i)
        {
            if (value == original[i][l])
            {
                conflicts += weightOriginalConflict;
            }
            else if (value == solution[i][l])
            {
                conflicts++;
            }
        }
    }
    return conflicts;
}

int Sudoku::getConflictsRows()
{
    int conflicts = 0;
    for (size_t i = 0; i < solution.size(); i++)
    {
        std::vector<int> hist(solution.size(), -1);
        for (size_t j = 0; j < solution[i].size(); j++)
        {
            if (original[i][j] != 0 && hist[original[i][j] - 1] != -1)
            {
                hist[original[i][j] - 1] += weightOriginalConflict;
            }
            else
            {
                hist[solution[i][j] - 1]++;
            }
        }
        conflicts += getConflicts(hist);
    }
    return conflicts;
}

int Sudoku::getConflictsCols()
{
    int conflicts = 0;
    for (size_t i = 0; i < solution.size(); i++)
    {
        std::vector<int> hist(solution.size(), -1);
        for (size_t j = 0; j < solution[i].size(); j++)
        {
            if (original[j][i] != 0 && hist[original[j][i] - 1] != -1)
            {
                hist[original[j][i] - 1] += weightOriginalConflict;
            }
            else
            {
                hist[solution[j][i] - 1]++;
            }
        }
        conflicts += getConflicts(hist);
    }
    return conflicts;
}

int Sudoku::getConflictsRowsAndCols()
{
    int conflicts = 0;
    for (size_t i = 0; i < solution.size(); i++)
    {
        std::vector<int> hist1(solution.size(), -1);
        std::vector<int> hist2(solution.size(), -1);
        for (size_t j = 0; j < solution[i].size(); j++)
        {
            hist1[solution[j][i] - 1]++;
            hist2[solution[i][j] - 1]++;
        }

        for (size_t j = 0; j < solution[i].size(); j++)
        {
            if (original[j][i] != 0 && hist1[original[j][i] - 1] > 0)
            {
                hist1[original[j][i] - 1] += weightOriginalConflict;
            }
            if (original[i][j] != 0 && hist2[original[i][j] - 1] > 0)
            {
                hist2[original[i][j] - 1] += weightOriginalConflict;
            }
        }
        conflicts += getConflicts(hist1);
        conflicts += getConflicts(hist2);
    }
    return conflicts;
}

int Sudoku::getConflictsSquare()
{
    int conflicts = 0;
    for (size_t k = 0; k < solution.size(); k += step)
    {
        for (size_t l = 0; l < solution.size(); l += step)
        {
            std::vector<int> hist(solution.size(), -1);
            for (size_t i = k; i < step + k; i++)
            {
                for (size_t j = l; j < step + l; j++)
                {
                    if (original[i][j] != 0 && hist[original[i][j] - 1] != -1)
                    {
                        hist[original[i][j] - 1] += weightOriginalConflict;
                    }
                    else
                    {
                        hist[solution[i][j] - 1]++;
                    }
                }
            }
            conflicts += getConflicts(hist);
        }
    }
    return conflicts;
}

size_t Sudoku::stochasticLocalSearch(size_t repetitions)
{
    int iTolerance = 0, tolerance = repetitions * .20;
    int lastConflicts, conflicts;
    lastConflicts = conflicts = getConflicts();
    for (size_t i = 0; i < repetitions; i++)
    {
        std::vector<bool> improveSquare(solution.size());
        size_t square = 0;
        for (size_t k = 0; k < solution.size(); k += step)
        {
            for (size_t l = 0; l < solution.size(); l += step, square++)
            {
                improveSquare[square] = stochasticLocalSearchSquare(k, l, step);
            }
        }
        if (find(improveSquare.begin(), improveSquare.end(), true) == improveSquare.end())
        {
            return i;
        }
        std::swap(conflicts, lastConflicts);
        conflicts = getConflicts();
        //std::cout << "i: " << i << " c: " << conflicts << std::endl;
        iTolerance = conflicts == lastConflicts ? iTolerance + 1 : 0;
        if (iTolerance > tolerance)
        {
            fitness = conflicts;
            return i;
        }
    }
    fitness = conflicts;
    return repetitions;
}

bool Sudoku::stochasticLocalSearchSquare(int k, int l, size_t step)
{
    int conflicts = getConflicts();
    bool improveSquare = false;
    size_t m = 1;
    for (size_t i = k; i < k + step; i++)
    {
        for (size_t j = l; j < l + step && m < solution.size(); j++, m++)
        {

            if (original[i][j] == 0)
            {
                bool improve = false;
                std::vector<int> options(solution.size() - m);
                std::iota(options.begin(), options.end(), m);
                std::random_shuffle(options.begin(), options.end());
                while (!options.empty() && !improve)
                {
                    int ii = k + options.back() / step, jj = l + options.back() % step;
                    if (original[ii][jj] == 0)
                    {
                        std::swap(solution[ii][jj], solution[i][j]);
                        int tempConflicts = getConflicts();
                        if (tempConflicts > conflicts)
                        {
                            std::swap(solution[ii][jj], solution[i][j]);
                        }
                        else
                        {
                            improveSquare = improve = true;
                            conflicts = tempConflicts;
                        }
                    }
                    options.pop_back();
                }
            }
        }
    }
    return improveSquare;
}

size_t Sudoku::stochasticLocalSearchAll(size_t repetitions)
{
    conflictsTable.resize(original.size(), std::vector<int>(original.size()));
    int iTolerance = 0, tolerance = repetitions * .25;
    int lastConflicts, conflicts;
    lastConflicts = getConflicts();
    std::vector<int> blocks(original.size());
    std::iota(blocks.begin(), blocks.end(), 0);
    for (size_t i = 0; i < repetitions; i++)
    {
        std::random_shuffle(blocks.begin(), blocks.end());
        for (auto j : blocks)
        {
            int k = (j / step) * step, l = (j % step) * step;
            stochasticLocalSearchAllSquare(k, l);
        }
        conflicts = getConflicts();
        if (conflicts == 0)
        {
            return i;
        }

        iTolerance = conflicts == lastConflicts ? iTolerance + 1 : 0;
        if (tolerance < iTolerance)
        {
            return i;
        }
        lastConflicts = conflicts;
    }
    return repetitions;
}

bool Sudoku::stochasticLocalSearchAllSquare(size_t k, size_t l)
{
    std::vector<unsigned short> *freeCells = &tableFreeCells[l / step + k];
    std::vector<unsigned short> solutionBlock;
    std::vector<unsigned short> zeroVec(original.size(), 0);
    setSquare(zeroVec, k, l);
    int index, best = 100000;
    size_t cell = 0;
    for (size_t i = k; i < k + step; i++)
    {
        for (size_t j = l; j < l + step; j++, cell++)
        {
            for (auto option : missingNumbersTable[l / step + k])
            {
                if (original[i][j] == 0)
                {
                    conflictsTable[cell][option - 1] = getConflicts(option, i, j);
                }
            }
        }
    }
    permutationsBlock = &permutationsPerBlock[l / step + k];
    std::vector<size_t> permutationsIndex(permutationsBlock->size());
    std::iota(permutationsIndex.begin(), permutationsIndex.end(), 0);
    std::random_shuffle(permutationsIndex.begin(), permutationsIndex.end());
    for (auto j : permutationsIndex)
    {
        int conflicts = 0;
        for (size_t i = 0; i < (*permutationsBlock)[j].size(); i++)
        {
            conflicts += conflictsTable[(*freeCells)[i]][(*permutationsBlock)[j][i] - 1];
        }
        if (conflicts < best)
        {
            best = conflicts;
            index = j;
        }
    }
    solutionBlock = (*permutationsBlock)[index];
    setSquare(solutionBlock, k, l);
    return true;
}

void Sudoku::simulatedAnnealing(double t, double tMin)
{
    std::vector<unsigned short> options;
    int deltaE;
    int k, l, i1, i2, j1, j2;
    int fitnessNeighbour;
    int fitnessActual = getConflictsRowsAndCols();
    while (tMin < t)
    {
        int i = 0;
        while (i++ < 100)
        {
            std::cout << fitnessActual << " " << t << std::endl;
            do
            {
                k = (rand() % step) * step;
                l = (rand() % step) * step;
                options = tableFreeCells[l / step + k];
            } while (options.size() < 2);
            std::random_shuffle(options.begin(), options.end());
            i1 = k + options[0] / step, j1 = l + options[0] % step;
            i2 = k + options[1] / step, j2 = l + options[1] % step;
            std::swap(solution[i1][j1], solution[i2][j2]);
            fitnessNeighbour = getConflictsRowsAndCols();
            deltaE = fitnessNeighbour - fitnessActual;
            if (deltaE <= 0 || (rand() % 100) / 100.0 < exp(-deltaE / t))
            {
                fitnessActual = fitnessNeighbour;
            }
            else
            {
                std::swap(solution[i1][j1], solution[i2][j2]);
            }
            if (fitnessActual == 0)
            {
                std::cout << "Resuelto\n";
                return;
            }
        }
        t *= .999; //actualziar T
    }
}

void Sudoku::setFitness()
{
    fitness = getConflictsRowsAndCols();
}

void Sudoku::mutate(double probability)
{
    for (size_t k = 0; k < solution.size(); k += step)
    {
        for (size_t l = 0; l < solution.size(); l += step)
        {
            if (randProbability(gen) < probability)
            {
                std::vector<unsigned short> options = tableFreeCells[l / step + k];
                std::random_shuffle(options.begin(), options.end());
                int i1 = k + options[0] / step, j1 = l + options[0] % step;
                int i2 = k + options[1] / step, j2 = l + options[1] % step;
                std::swap(solution[i1][j1], solution[i2][j2]);
            }
        }
    }
}

void Sudoku::cross(const Individual &partner, const size_t pos)
{
    size_t k = (pos / step) * step, l = (pos % step) * step;
    const std::vector<std::vector<unsigned short>> &solP = static_cast<const Sudoku &>(partner).getSolution();
    for (; k < solution.size(); k += step)
    {
        for (; l < solution.size(); l += step)
        {
            for (size_t i = k; i < k + step; i++)
            {
                for (size_t j = l; j < l + step; j++)
                {
                    solution[i][j] = solP[i][j];
                }
            }
        }
    }
}

void Sudoku::setDCN(const std::vector<Sudoku> &survivors)
{
    dcn = getDistance(survivors[0]);
    for (size_t i = 1; i < survivors.size(); i++)
    {
        double temp = getDistance(survivors[i]);
        if (temp < dcn)
        {
            dcn = temp;
        }
    }
}

double Sudoku::getDistance(const Sudoku &sud)
{
    const std::vector<std::vector<unsigned short>> solution2 = sud.getSolution();    
    double dcn = 0;
    for (size_t i = 0; i < solution.size(); i++)
    {
        for (size_t j = 0; j < solution.size(); j++)
        {
            if (solution[i][j] != solution2[i][j])
            {
                dcn++;
            }
        }
    }
    return dcn;
}

void Sudoku::initRandom()
{
    std::random_device rd;
    gen.seed(rd());
    createRandomSolution();
    setFitness();
}

size_t Sudoku::getGenotypeLength() const
{
    return original.size();
}
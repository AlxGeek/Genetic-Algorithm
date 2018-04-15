#include <iostream>
#include <ctime>
#include <chrono>
#include <bitset>

#include "Sudoku.hpp"
#include "GeneticAlgorithm.hpp"



template<class T>
void runTests(std::ostream &output, GeneticAlgorithm<T> &ga, int repetitions)
{
    int durationInit = 0, durationSearch = 0;
    int average = 0, best = 10000, nSolve = 0;    
    for (int i = 0; i < repetitions; i++)
    {
        int c;
        auto start = std::chrono::steady_clock::now();
        ga.initPoblation();
        durationInit += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
        output << "Test: " << i << " Initial fitness: " << ga.getBest().getFitness() << " ";
        start = std::chrono::steady_clock::now();
        output << "Generations: " << ga.run(1800) << " ";
        durationSearch += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
        output << "Final fitness: " << (c = ga.getBest().getFitness()) << std::endl;
        if (c < best)
        {
            best = c;
        }
        if (c == 0)
        {
            nSolve++;
        }
        average += c;
    }
    output << "Average: " << average / (float)repetitions << " Best fitness: " << best << " Success rate: " << nSolve / (float)repetitions * 100. << "%" << std::endl;
    output << "Init population duration(ms): " << durationInit / (float)repetitions << " Optimization duration(ms): " << durationSearch / (float)repetitions << std::endl;
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        std::cout << "Uso: programa sudoku pruebas" << std::endl;
        return -1;
    }
    std::srand(unsigned(std::time(0)));
    Sudoku sudoku(argv[1]);
    GeneticAlgorithm<Sudoku> ga(sudoku, 50, 1, 80, 0);    
    runTests(std::cout, ga, atoi(argv[2]));
    return 0;
}
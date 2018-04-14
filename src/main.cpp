#include <iostream>
#include <ctime>
#include <chrono>
#include <bitset>

#include "Sudoku.hpp"
#include "GeneticAlgorithm.hpp"

template<class T>
void runTestsToFile(GeneticAlgorithm<T> &ga, int repetitions, const char *filename)
{
    int durationInit = 0, durationSearch = 0;
    int average = 0, best = 10000, nSolve = 0;
    ;
    for (int i = 0; i < repetitions; i++)
    {
        int c;
        auto start = std::chrono::steady_clock::now();
        ga.initPoblation();
        durationInit += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
        start = std::chrono::steady_clock::now();
        ga.run(1800);
        durationSearch += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
        c = ga.getBest().getFitness();
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
    std::ofstream file(filename);
    file << " & " << average / (float)repetitions << " & " << best << " & " << nSolve / (float)repetitions * 100. << "%"
         << " & " << durationInit / (float)repetitions << " ms & " << durationSearch / (float)repetitions << " ms \\\\" << std::endl;
    file.close();
}

template<class T>
void runTests(GeneticAlgorithm<T> &ga, int repetitions)
{
    int durationInit = 0, durationSearch = 0;
    int average = 0, best = 10000, nSolve = 0;
    ;
    for (int i = 0; i < repetitions; i++)
    {
        int c;
        auto start = std::chrono::steady_clock::now();
        ga.initPoblation();
        durationInit += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
        std::cout << "Prueba: " << i << " Error inicial: " << ga.getBest().getFitness() << " ";
        start = std::chrono::steady_clock::now();
        std::cout << "Generaciones: " << ga.run(1800) << " ";
        durationSearch += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
        std::cout << "Error final: " << (c = ga.getBest().getFitness()) << std::endl;
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
    std::cout << "Promedio: " << average / (float)repetitions << " Mejor: " << best << " Ratio de exito: " << nSolve / (float)repetitions * 100. << "%" << std::endl;
    std::cout << "Duracion crear solucion(ms): " << durationInit / (float)repetitions << " Duracion busqueda local(ms): " << durationSearch / (float)repetitions << std::endl;
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
    runTests(ga, atoi(argv[2]));
    return 0;
}
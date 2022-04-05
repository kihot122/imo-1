#include "main.hpp"
#include "greedy_helpers.hpp"

#include <iostream>


std::vector<int> VertexGreedy(std::vector<std::vector<int>> Matrix, std::vector<int> cycles)
{
    auto CycleA = std::vector<int>(cycles.begin(), cycles.begin() + cycles.size() / 2);
	auto CycleB = std::vector<int>(cycles.begin() + cycles.size() / 2, cycles.end());

    bool cycleNotOptimalA = true, cycleNotOptimalB = true, cyclesNotOptimal = true;

    while (cycleNotOptimalA || cycleNotOptimalB || cyclesNotOptimal)
    {
        cycleNotOptimalA = localCycleOptimisation(Matrix, CycleA, 1, false);
        cycleNotOptimalB = localCycleOptimisation(Matrix, CycleB, 1, false);
        cyclesNotOptimal = CrossGreedy(Matrix, CycleA, CycleB, 1);
    }
    
    
    CycleA.insert( CycleA.end(), CycleB.begin(), CycleB.end());
    return CycleA;
}



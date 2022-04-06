#include "main.hpp"
#include "greedy_helpers.hpp"

#include <chrono>

std::vector<int> VertexRandom(std::vector<std::vector<int>> Matrix, std::vector<int> cycles, uint64_t timeNs)
{
    auto CycleA = std::vector<int>(cycles.begin(), cycles.begin() + cycles.size() / 2);
	auto CycleB = std::vector<int>(cycles.begin() + cycles.size() / 2, cycles.end());

    bool cycleNotOptimalA = true, cycleNotOptimalB = true, cyclesNotOptimal = true;

    auto TimeA = std::chrono::high_resolution_clock::now();

    while (std::chrono::duration<size_t, std::nano>(std::chrono::high_resolution_clock::now() - TimeA).count() < timeNs)
    {
        cycleNotOptimalA = localCycleRandom(Matrix, CycleA, 1, false);
        cycleNotOptimalB = localCycleRandom(Matrix, CycleB, 1, false);
        cyclesNotOptimal = CrossGreedyRandom(Matrix, CycleA, CycleB, 1);
    }
    
    
    CycleA.insert( CycleA.end(), CycleB.begin(), CycleB.end());
    return CycleA;
}



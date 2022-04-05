#include <array>
#include <vector>
#include <iostream>
#include <random>
#include <chrono>

#include "main.hpp"


std::array<int, 3> getFromCyclePrevMiddleNext(std::vector<int> &Cycle, int middleIndex)
{
    int prev = middleIndex - 1 < 0 ? Cycle.size() - 1 : middleIndex - 1;
    int next = middleIndex + 1 == Cycle.size() ? 0 : middleIndex + 1;
    
    return std::array<int, 3>{Cycle[prev], Cycle[middleIndex],Cycle[next]};
}


int crossDeltaProfit(std::vector<std::vector<int>> &Matrix, std::vector<int> &CycleA, std::vector<int> &CycleB, int swapIndexA, int swapIndexB)
{   
    int cycleSizeA = CycleA.size();
    int cycleSizeB = CycleB.size();

    std::array<int, 3> cyclePointsA = getFromCyclePrevMiddleNext(CycleA, swapIndexA);
    std::array<int, 3> cyclePointsB = getFromCyclePrevMiddleNext(CycleB, swapIndexB);

    int beforePartValue = 
        Matrix[cyclePointsA[1]][cyclePointsA[0]] + Matrix[cyclePointsA[1]][cyclePointsA[2]] + 
        Matrix[cyclePointsB[1]][cyclePointsB[0]] + Matrix[cyclePointsB[1]][cyclePointsB[2]];

    int afterPartValue = 
        Matrix[cyclePointsA[1]][cyclePointsB[0]] + Matrix[cyclePointsA[1]][cyclePointsB[2]] + 
        Matrix[cyclePointsB[1]][cyclePointsA[0]] + Matrix[cyclePointsB[1]][cyclePointsA[2]];

    return afterPartValue - beforePartValue;
}


int validateCycle(std::vector<std::vector<int>> &matrix, std::vector<int> &cycle)
{
    int sumValue = matrix[cycle[0]][cycle[cycle.size()-1]];
    for (int i = 1; i < cycle.size(); i++)
    {
        sumValue += matrix[cycle[i-1]][cycle[i]];
    }
    return sumValue;
}


std::vector<int> CrossGreedy(std::vector<std::vector<int>> Matrix, std::vector<int> Cycles, int changesCount)
{
    auto CycleA = std::vector<int>(Cycles.begin(), Cycles.begin() + Cycles.size() / 2);
	auto CycleB = std::vector<int>(Cycles.begin() + Cycles.size() / 2, Cycles.end());

    
    int localDelta;
    bool swapped;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng = std::default_random_engine(seed);

    std::vector<int> randomIndex1, randomIndex2;
    for (int i = 0; i < CycleA.size(); i++)
    {
        randomIndex1.push_back(i);
    }

    for (int i = 0; i < CycleB.size(); i++)
    {
        randomIndex2.push_back(i);
    }



    for (int changes = 0; changes < changesCount; changes++)
    {
        swapped = false;
        std::shuffle(std::begin(randomIndex1), std::end(randomIndex1), rng);
        std::shuffle(std::begin(randomIndex2), std::end(randomIndex2), rng);

        for (int a : randomIndex1)
        {
            for (int b : randomIndex2)
            {
                localDelta = crossDeltaProfit(Matrix, CycleA, CycleB, a, b);
                
                if (localDelta < 0)
                {
                    std::swap(CycleA[a], CycleB[b]);
                    swapped = true;
                    break;
                }

            }
            if (swapped)
            {
                break;
            }
        }

        if (not swapped)
        {
            break;
        }
    }

    CycleA.insert( CycleA.end(), CycleB.begin(), CycleB.end());
    return CycleA;
}
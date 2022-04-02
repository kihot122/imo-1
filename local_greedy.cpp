#include <vector>
#include <array>
#include <iostream>
#include <algorithm>

#include "main.hpp"


// std::array<int, 3> getFromCyclePrevMiddleNext(std::vector<int> &Cycle, int middleIndex)
// {
//     int prev = middleIndex - 1 < 0 ? Cycle.size() - 1 : middleIndex - 1;
//     int next = middleIndex + 1 == Cycle.size() ? 0 : middleIndex + 1;
    
//     return std::array<int, 3>{Cycle[prev], Cycle[middleIndex],Cycle[next]};
// }


int pointsSwapProfit(std::vector<std::vector<int>> &matrix, std::vector<int> &cycle, int swapIndexA, int swapIndexB)
{
    std::array<int, 3> cyclePointsA = getFromCyclePrevMiddleNext(cycle, swapIndexA);
    std::array<int, 3> cyclePointsB = getFromCyclePrevMiddleNext(cycle, swapIndexB);

    int beforePartValue = 
        matrix[cyclePointsA[1]][cyclePointsA[0]] + matrix[cyclePointsA[1]][cyclePointsA[2]] + 
        matrix[cyclePointsB[1]][cyclePointsB[0]] + matrix[cyclePointsB[1]][cyclePointsB[2]];

    int afterPartValue = 
        matrix[cyclePointsA[1]][cyclePointsB[0]] + matrix[cyclePointsA[1]][cyclePointsB[2]] + 
        matrix[cyclePointsB[1]][cyclePointsA[0]] + matrix[cyclePointsB[1]][cyclePointsA[2]];

    return afterPartValue - beforePartValue;
}


int sectionsSwapProfit(std::vector<std::vector<int>> &matrix, std::vector<int> &cycle, int sectionIndexA, int sectionIndexB)
{
    std::array<int, 3> cyclePointsA = getFromCyclePrevMiddleNext(cycle, sectionIndexA);
    std::array<int, 3> cyclePointsB = getFromCyclePrevMiddleNext(cycle, sectionIndexB);

    int beforePartValue = matrix[cyclePointsA[0]][cyclePointsA[1]] + matrix[cyclePointsB[0]][cyclePointsB[1]];

    int afterPartValue = matrix[cyclePointsA[0]][cyclePointsB[0]] + matrix[cyclePointsA[1]][cyclePointsB[1]];

    return afterPartValue - beforePartValue;
}


void swapLocalPoints(std::vector<int> &cycle, int indexA, int indexB)
{
    std::swap(cycle[indexA], cycle[indexB]);
}


void swapLocalSections(std::vector<int> &cycle, int sectionIndexA, int sectionIndexB)
{
    if (sectionIndexA < sectionIndexB)
        std::reverse(std::begin(cycle)+sectionIndexA, std::begin(cycle)+sectionIndexB+1);
    else
        std::reverse(std::begin(cycle)+sectionIndexB, std::begin(cycle)+sectionIndexA+1);
}


bool localCycleOptimisation(std::vector<std::vector<int>> &Matrix, std::vector<int> &cycle, int changesCount)
{
    int localDelta;
    bool swapped;
    int cycleSize = cycle.size();

    for (int changes = 0; changes < changesCount; changes++)
    {
        swapped = false;
        for (int a = 0; a < cycleSize; a++)
        {
            for (int b = 0; b < cycleSize; b++)
            {
                if (a == b)
                    continue;

                localDelta = pointsSwapProfit(Matrix, cycle, a, b);
                if (localDelta > 0)
                {
                    swapLocalPoints(cycle, a, b);

                    a = cycleSize;
                    b = cycleSize;
                    swapped = true;
                    break;
                }

                localDelta = sectionsSwapProfit(Matrix, cycle, a, b);
                if (localDelta > 0)
                {
                    swapLocalSections(cycle, a, b);

                    a = cycleSize;
                    b = cycleSize;
                    swapped = true;
                    break;
                }
            }
        }

        if (not swapped)
            return false;
    }

    return true;
}


std::vector<int> localCyclesOptimisation(std::vector<std::vector<int>> Matrix, std::vector<int> cycles, int changesCount)
{
    auto CycleA = std::vector<int>(cycles.begin(), cycles.begin() + cycles.size() / 2);
	auto CycleB = std::vector<int>(cycles.begin() + cycles.size() / 2, cycles.end());

    bool cycleOptimalA = false, cycleOptimalB = false;


    for (int count = 0; count < changesCount; count++)
    {
        if (not cycleOptimalA) 
            localCycleOptimisation(Matrix, CycleA, 1);
        
        if (not cycleOptimalB) 
            localCycleOptimisation(Matrix, CycleB, 1);

        if (cycleOptimalA and cycleOptimalB)
            break;

    }

    CycleA.insert( CycleA.end(), CycleB.begin(), CycleB.end());
    return CycleA;
}

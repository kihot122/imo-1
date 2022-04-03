#include <vector>
#include <array>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

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

    int A1Min = cyclePointsA[0]; 
    int A1 =  cyclePointsA[1]; 
    int A1Max = cyclePointsA[2]; 
    int A2Min = cyclePointsB[0]; 
    int A2 = cyclePointsB[1]; 
    int A2Max = cyclePointsB[2];
    int DeltaA = 0;

    //(A1-1 to A2) + (A1+1 to A2) + (A2-1 to A1) + (A2+1 to A1)
    //- (A1-1 to A1) - (A1+1 to A1) - (A2-1 to A2) - (A2+1 to A2)
    if (A1Max != A2 and A2Max != A1)
        DeltaA = matrix[A1Min][A2] + matrix[A1Max][A2] + matrix[A2Min][A1] + matrix[A2Max][A1] - matrix[A1Min][A1] - matrix[A1Max][A1] -
                    matrix[A2Min][A2] - matrix[A2Max][A2];
    //(A1-1 to A2) + (A2+1 to A1)
    //- (A1-1 to A1) - (A2+1 to A2)
    else if (A2Max != A1)
        DeltaA = matrix[A1Min][A2] + matrix[A2Max][A1] - matrix[A1Min][A1] - matrix[A2Max][A2];
    //(A1 to A2-1) + (A2 to A1+1)
    //- (A1 to A1+1) - (A2 to A2-1)
    else
        DeltaA = matrix[A1][A2Min] + matrix[A2][A1Max] - matrix[A1][A1Max] - matrix[A2][A2Min];

    return DeltaA;
}


int sectionsSwapProfit(std::vector<std::vector<int>> &matrix, std::vector<int> &cycle, int sectionIndexA, int sectionIndexB)
{
    std::array<int, 3> cyclePointsA = getFromCyclePrevMiddleNext(cycle, sectionIndexA);
    std::array<int, 3> cyclePointsB = getFromCyclePrevMiddleNext(cycle, sectionIndexB);

    int beforePartValue = matrix[cyclePointsA[1]][cyclePointsA[2]] + matrix[cyclePointsB[1]][cyclePointsB[2]];

    int afterPartValue = matrix[cyclePointsA[1]][cyclePointsB[1]] + matrix[cyclePointsA[2]][cyclePointsB[2]];

    // DeltaA = Matrix[CycleA[A1]][CycleA[A2]] + Matrix[CycleA[A1Max]][CycleA[A2Max]] - Matrix[CycleA[A1]][CycleA[A1Max]] - Matrix[CycleA[A2]][CycleA[A2Max]];

    return afterPartValue - beforePartValue;
}


void swapLocalPoints(std::vector<int> &cycle, int indexA, int indexB)
{
    std::swap(cycle[indexA], cycle[indexB]);
}


void swapLocalSections(std::vector<int> &cycle, int sectionIndexA, int sectionIndexB)
{
    std::reverse(cycle.begin() + sectionIndexA + 1, cycle.begin() + sectionIndexB + 1);
}


bool localCycleOptimisation(std::vector<std::vector<int>> &Matrix, std::vector<int> &cycle, int changesCount)
{
    int localDelta;
    bool swapped;
    int cycleSize = cycle.size();

    std::vector<int> randomIndex1, randomIndex2;
    for (int i = 0; i < cycleSize; i++)
    {
        randomIndex1.push_back(i);
        randomIndex2.push_back(i);
    }
    
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng = std::default_random_engine(seed);

    for (int changes = 0; changes < changesCount; changes++)
    {
        swapped = false;

        
        // rng = std::default_random_engine(seed);
        std::shuffle(std::begin(randomIndex1), std::end(randomIndex1), rng);
        std::shuffle(std::begin(randomIndex2), std::end(randomIndex2), rng);

        for (int a : randomIndex1)
        {
            for (int b : randomIndex2)
            {
                if (a == b)
                    continue;

                localDelta = pointsSwapProfit(Matrix, cycle, a, b);
                if (localDelta < 0)
                {
                    swapLocalPoints(cycle, a, b);

                    swapped = true;
                    break;
                }

                localDelta = sectionsSwapProfit(Matrix, cycle, a, b);
                if (localDelta < 0)
                {
                    swapLocalSections(cycle, a, b);

                    swapped = true;
                    break;
                }
            }
            if (swapped)
                break;
        }

        if (not swapped)
        {
            std::cout << changes << "\n";
            return false;
        }
            
    }

    return true;
}


std::vector<int> LocalGreedy(std::vector<std::vector<int>> Matrix, std::vector<int> cycles, int changesCount)
{
    auto CycleA = std::vector<int>(cycles.begin(), cycles.begin() + cycles.size() / 2);
	auto CycleB = std::vector<int>(cycles.begin() + cycles.size() / 2, cycles.end());

    bool cycleOptimalA = true, cycleOptimalB = true;

    std::cout << localCycleOptimisation(Matrix, CycleA, changesCount/2)  << "\n";

    std::cout << localCycleOptimisation(Matrix, CycleB, changesCount/2)  << "\n";
    
    // bool cycleOptimalA = true, cycleOptimalB = true;
    // for (int count = 0; count < changesCount; count++)
    // {
    //     if (cycleOptimalA) 
    //         cycleOptimalA = localCycleOptimisation(Matrix, CycleA, 1);
        
    //     if (cycleOptimalB) 
    //         cycleOptimalB = localCycleOptimisation(Matrix, CycleB, 1);

    //     if (not (cycleOptimalA and cycleOptimalB))
    //         break;

    // }

    CycleA.insert( CycleA.end(), CycleB.begin(), CycleB.end());
    return CycleA;
}

#include <array>
#include <iostream>
#include <random>
#include <chrono>

#include "main.hpp"
#include "greedy_helpers.hpp"


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


bool CrossGreedy(std::vector<std::vector<int>> &Matrix, std::vector<int> &CycleA, std::vector<int> &CycleB, int changesCount)
{
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
                    std::cout << localDelta << "\n";
                    std::cout << CycleA[a] << "\n";
                    std::cout << CycleB[b] << "\n";

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
            return false;
        }
    }

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------


int localPointsSwapProfit(std::vector<std::vector<int>> &matrix, std::vector<int> &cycle, int swapIndexA, int swapIndexB)
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


int localSectionsSwapProfit(std::vector<std::vector<int>> &matrix, std::vector<int> &cycle, int sectionIndexA, int sectionIndexB)
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


bool localCycleOptimisation(std::vector<std::vector<int>> &Matrix, std::vector<int> &cycle, int changesCount, bool sections)
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

                if (not sections)
                {
                    localDelta = localPointsSwapProfit(Matrix, cycle, a, b);
                    if (localDelta < 0)
                    {
                        swapLocalPoints(cycle, a, b);

                        swapped = true;
                        break;
                    }
                }
                else
                {
                    localDelta = localSectionsSwapProfit(Matrix, cycle, a, b);
                    if (localDelta < 0)
                    {
                        swapLocalSections(cycle, a, b);

                        swapped = true;
                        break;
                    }
                }
            }
            if (swapped)
                break;
        }

        if (not swapped)
        {
            return false;
        }
    }

    return true;
}


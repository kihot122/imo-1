#include <algorithm>
#include <array>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <iostream>


std::array<int, 3> getFromCyclePrevMiddNext(std::vector<int> &Cycle, int middleIndex)
{
	int prev = middleIndex - 1 < 0 ? Cycle.size() - 1 : middleIndex - 1;
	int next = middleIndex + 1 == Cycle.size() ? 0 : middleIndex + 1;

	return std::array<int, 3>{Cycle[prev], Cycle[middleIndex], Cycle[next]};
}

int localSectionsSwapDelta(const std::vector<std::vector<int>> &matrix, std::vector<int> &cycle, int sectionIndexA, int sectionIndexB)
{
	std::array<int, 3> cyclePointsA = getFromCyclePrevMiddNext(cycle, sectionIndexA);
	std::array<int, 3> cyclePointsB = getFromCyclePrevMiddNext(cycle, sectionIndexB);

	int beforePartValue = matrix[cyclePointsA[1]][cyclePointsA[2]] + matrix[cyclePointsB[1]][cyclePointsB[2]];
	int afterPartValue = matrix[cyclePointsA[1]][cyclePointsB[1]] + matrix[cyclePointsA[2]][cyclePointsB[2]];

	// DeltaA = Matrix[CycleA[A1]][CycleA[A2]] + Matrix[CycleA[A1Max]][CycleA[A2Max]] - Matrix[CycleA[A1]][CycleA[A1Max]] - Matrix[CycleA[A2]][CycleA[A2Max]];
	return afterPartValue - beforePartValue;
}

std::array<int, 3> bestLocalSwap(const std::vector<std::vector<int>> &matrix, std::vector<int> &cycle, std::vector<bool> &removed)
{
    std::array<int, 3> bestSwap = {0, NULL, NULL};
    int localDelta = 0;

    for(int i = 0; i < removed.size(); i++) if(removed[i]) for(int j = i+1; j < removed.size(); j++) if(removed[j])
    {
        localDelta = localSectionsSwapDelta(matrix, cycle, i, j);
        if (localDelta < bestSwap[0])
        {
            bestSwap[0] = localDelta;
            bestSwap[1] = i;
            bestSwap[2] = j;
        }
    }

    return bestSwap;
}

void swapLocSections(std::vector<int> &cycle, int sectionIndexA, int sectionIndexB)
{
	std::reverse(cycle.begin() + sectionIndexA + 1, cycle.begin() + sectionIndexB + 1);
}

void steepRepair(const std::vector<std::vector<int>> &matrix, std::vector<int> &mycleA, std::vector<int> &mycleB, std::vector<bool> &removedA, std::vector<bool> &removedB)
{
    while (true)
    {
        std::array<int, 3> swapA = bestLocalSwap(matrix, mycleA, removedA);
        std::array<int, 3> swapB = bestLocalSwap(matrix, mycleB, removedB);
        if (swapA[0] < swapB[0])
        {   
            if (swapA[0] >= 0) break;
            swapLocSections(mycleA, swapA[1], swapA[2]);
        } else
        {
            if (swapB[0] >= 0) break;
            swapLocSections(mycleB, swapB[1], swapB[2]);
        }
    }
}
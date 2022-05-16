#include <algorithm>
#include <array>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <iostream>

#include "main.hpp"

enum class ILSDestroyRepair::ExchangeType : bool
{
    LOCAL,
    CROSS,
};

void ILSDestroyRepair::edgeSteep(std::vector<std::vector<int>> &matrix, std::vector<int> &cycles)
{
    std::array cyclesArr{std::vector<int>(cycles.begin(), cycles.begin() + cycles.size() / 2), std::vector<int>(cycles.begin() + cycles.size() / 2, cycles.end())};
	auto &cycleA = cyclesArr[0];
	auto &cycleB = cyclesArr[1];

	int minDelta;
	do
	{
		minDelta = 0;
		int delta;
		int choiceA, choiceB, cycleIndex;
		ExchangeType type;

		for (int i = 0; auto &&cycle : cyclesArr)
		{
			for (int A = 0; A < cycle.size(); A++)
			{
				for (int B = A + 1; B < cycle.size(); B++)
				{
					int Size = static_cast<int>(cycle.size());

					int AMin = A - 1 < 0 ? Size - 1 : A - 1;
					int BMin = B - 1 < 0 ? Size - 1 : B - 1;

					int AMax = A + 1 == Size ? 0 : A + 1;
					int BMax = B + 1 == Size ? 0 : B + 1;

					//(A to B) + (A+1 to B+1) - (A to A+1) - (B + B+1)
					delta = matrix[cycle[A]][cycle[B]] + matrix[cycle[AMax]][cycle[BMax]] - matrix[cycle[A]][cycle[AMax]] - matrix[cycle[B]][cycle[BMax]];

					if (delta < minDelta)
					{
						minDelta = delta;
						choiceA = A;
						choiceB = B;
						cycleIndex = i;
						type = ExchangeType::LOCAL;
					}
				}
			}
			i++;
		}

		for (int A = 0; A < cycleA.size(); A++)
		{
			for (int B = 0; B < cycleB.size(); B++)
			{
				int sizeA = static_cast<int>(cycleA.size());
				int sizeB = static_cast<int>(cycleB.size());

				int AMin = A - 1 < 0 ? sizeA - 1 : A - 1;
				int BMin = B - 1 < 0 ? sizeB - 1 : B - 1;

				int AMax = A + 1 == sizeA ? 0 : A + 1;
				int BMax = B + 1 == sizeB ? 0 : B + 1;

				//(A-1 to B) + (A+1 to B) + (B-1 to A) + (B+1 to A)
				//- (A-1 to A) - (A+1 to A) - (B-1 to B) - (B+1 to B)
				delta = matrix[cycleA[AMin]][cycleB[B]] + matrix[cycleA[AMax]][cycleB[B]] + matrix[cycleB[BMin]][cycleA[A]] + matrix[cycleB[BMax]][cycleA[A]] - matrix[cycleA[AMin]][cycleA[A]] - matrix[cycleA[AMax]][cycleA[A]] -
						matrix[cycleB[BMin]][cycleB[B]] - matrix[cycleB[BMax]][cycleB[B]];

				if (delta < minDelta)
				{
					minDelta = delta;
					choiceA = A;
					choiceB = B;
					type = ExchangeType::CROSS;
				}
			}
		}

		if (minDelta < 0)
		{
			switch (type)
			{
				case ExchangeType::LOCAL:
				{
					std::reverse(cyclesArr[cycleIndex].begin() + choiceA + 1, cyclesArr[cycleIndex].begin() + choiceB + 1);
					break;
				}
				case ExchangeType::CROSS:
				{
					std::swap(cycleA[choiceA], cycleB[choiceB]);
					break;
				}
			}
		}

	} while (minDelta < 0);

    std::vector<int> temp = cyclesArr[0];
	temp.insert(temp.end(), cyclesArr[1].begin(), cyclesArr[1].end());

	cycles = temp;
}        

void ILSDestroyRepair::nearestGreedy(std::vector<std::vector<int>> &matrix, std::vector<int> &cycles, std::vector<bool> &left, int notAssignedCount)
{
    std::array cycle{std::vector<int>(cycles.begin(), cycles.begin() + cycles.size() / 2), std::vector<int>(cycles.begin() + cycles.size() / 2, cycles.end())};
    auto &CycleA = cycle[0];
    auto &CycleB = cycle[1];


	int pointsCount = matrix.size();
	int assigned = pointsCount - notAssignedCount;
	int closestId;
	int cycleIndex;
	while (assigned != pointsCount)
	{
		cycleIndex = assigned % 2;
		closestId = pointsCount;

		for (int j = 0; j < pointsCount; j++)
		{
			if (left[j])
			{
				if (closestId == pointsCount)
				{
					closestId = j;
					continue;
				}

				if (matrix[cycle[cycleIndex].back()][j] <= matrix[cycle[cycleIndex].back()][closestId])
				{
					closestId = j;
				}
			}
		}

		left[closestId] = false;
		cycle[cycleIndex].push_back(closestId);
		assigned++;
	}

    std::vector<int> temp = cycle[0];
	temp.insert(temp.end(), cycle[1].begin(), cycle[1].end());

    cycles = temp;
}

void ILSDestroyRepair::destroyRepairPerturbation(std::vector<std::vector<int>> &matrix, std::vector<int> &cycles, float deleteRatio)
{
    std::array cycleArray{std::vector<int>(cycles.begin(), cycles.begin() + cycles.size() / 2), std::vector<int>(cycles.begin() + cycles.size() / 2, cycles.end())};
    auto &cycleA = cycleArray[0];
    auto &cycleB = cycleArray[1];

    int deleteQuantity = (int)(cycles.size()*deleteRatio);
    std::vector<bool> left (cycles.size(), false);
    int toDeleteIx;
    int toDelete;

    srand(time(NULL) + rand());

    for (int i = 0; i < deleteQuantity; i++)
    {

        if (i%2 == 0)
        {   
            toDeleteIx = rand()%cycleA.size();
            toDelete = cycleA[toDeleteIx];
            cycleA.erase(cycleA.begin()+toDeleteIx);

        }
        else
        {
            toDeleteIx = rand()%cycleB.size();
            toDelete = cycleB[toDeleteIx];
            cycleB.erase(cycleB.begin()+toDeleteIx);

        }

        left[toDelete] = true;
    }

    std::vector<int> Temp = cycleArray[0];
	Temp.insert(Temp.end(), cycleArray[1].begin(), cycleArray[1].end());

	cycles = Temp;

    this->nearestGreedy(matrix, cycles, left, deleteQuantity);
}

int ILSDestroyRepair::cyclesLength(std::vector<std::vector<int>> &matrix, std::vector<int> &cycles)
{
    int length = 0;

	for (std::size_t i = 0; i < cycles.size() / 2 - 1; i++)
		length += matrix[cycles[i]][cycles[i + 1]];

	length += matrix[cycles[cycles.size() / 2 - 1]][cycles[0]];

	for (std::size_t i = cycles.size() / 2; i < cycles.size() - 1; i++)
		length += matrix[cycles[i]][cycles[i + 1]];

	length += matrix[cycles[cycles.size() - 1]][cycles[cycles.size() / 2]];

	return length;
}

std::vector<int> ILSDestroyRepair::ils(std::vector<std::vector<int>> matrix, std::vector<int> cycles, double timeS, int iterations)
{
    std::vector<int> x = cycles;
    std::vector<int> y = cycles;

    edgeSteep(matrix, x);

    auto timeBefore = std::chrono::high_resolution_clock::now();
    int c = 0;
    while (std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - timeBefore).count() <= timeS)
    {
        if (c >= iterations)
        {
            break;
        }

        y = x;

        destroyRepairPerturbation(matrix, y);
        edgeSteep(matrix, y);

        if (cyclesLength(matrix, x) > cyclesLength(matrix, y))
        {
            x = y;
        }
        c++;
    }

    return x;
}





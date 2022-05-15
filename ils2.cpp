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

void ILSDestroyRepair::edgeSteep(std::vector<std::vector<int>> &Matrix, std::vector<int> &Cycles)
{
    std::array CyclesArr{std::vector<int>(Cycles.begin(), Cycles.begin() + Cycles.size() / 2), std::vector<int>(Cycles.begin() + Cycles.size() / 2, Cycles.end())};
	auto &CycleA = CyclesArr[0];
	auto &CycleB = CyclesArr[1];

	int MinDelta;
	do
	{
		MinDelta = 0;
		int Delta;
		int ChoiceA, ChoiceB, CycleIndex;
		ExchangeType Type;

		for (int i = 0; auto &&Cycle : CyclesArr)
		{
			for (int A = 0; A < Cycle.size(); A++)
			{
				for (int B = A + 1; B < Cycle.size(); B++)
				{
					int Size = static_cast<int>(Cycle.size());

					int AMin = A - 1 < 0 ? Size - 1 : A - 1;
					int BMin = B - 1 < 0 ? Size - 1 : B - 1;

					int AMax = A + 1 == Size ? 0 : A + 1;
					int BMax = B + 1 == Size ? 0 : B + 1;

					//(A to B) + (A+1 to B+1) - (A to A+1) - (B + B+1)
					Delta = Matrix[Cycle[A]][Cycle[B]] + Matrix[Cycle[AMax]][Cycle[BMax]] - Matrix[Cycle[A]][Cycle[AMax]] - Matrix[Cycle[B]][Cycle[BMax]];

					if (Delta < MinDelta)
					{
						MinDelta = Delta;
						ChoiceA = A;
						ChoiceB = B;
						CycleIndex = i;
						Type = ExchangeType::LOCAL;
					}
				}
			}
			i++;
		}

		for (int A = 0; A < CycleA.size(); A++)
		{
			for (int B = 0; B < CycleB.size(); B++)
			{
				int SizeA = static_cast<int>(CycleA.size());
				int SizeB = static_cast<int>(CycleB.size());

				int AMin = A - 1 < 0 ? SizeA - 1 : A - 1;
				int BMin = B - 1 < 0 ? SizeB - 1 : B - 1;

				int AMax = A + 1 == SizeA ? 0 : A + 1;
				int BMax = B + 1 == SizeB ? 0 : B + 1;

				//(A-1 to B) + (A+1 to B) + (B-1 to A) + (B+1 to A)
				//- (A-1 to A) - (A+1 to A) - (B-1 to B) - (B+1 to B)
				Delta = Matrix[CycleA[AMin]][CycleB[B]] + Matrix[CycleA[AMax]][CycleB[B]] + Matrix[CycleB[BMin]][CycleA[A]] + Matrix[CycleB[BMax]][CycleA[A]] - Matrix[CycleA[AMin]][CycleA[A]] - Matrix[CycleA[AMax]][CycleA[A]] -
						Matrix[CycleB[BMin]][CycleB[B]] - Matrix[CycleB[BMax]][CycleB[B]];

				if (Delta < MinDelta)
				{
					MinDelta = Delta;
					ChoiceA = A;
					ChoiceB = B;
					Type = ExchangeType::CROSS;
				}
			}
		}

		if (MinDelta < 0)
		{
			switch (Type)
			{
				case ExchangeType::LOCAL:
				{
					std::reverse(CyclesArr[CycleIndex].begin() + ChoiceA + 1, CyclesArr[CycleIndex].begin() + ChoiceB + 1);
					break;
				}
				case ExchangeType::CROSS:
				{
					std::swap(CycleA[ChoiceA], CycleB[ChoiceB]);
					break;
				}
			}
		}

	} while (MinDelta < 0);

    std::vector<int> Temp = CyclesArr[0];
	Temp.insert(Temp.end(), CyclesArr[1].begin(), CyclesArr[1].end());

	Cycles = Temp;
}        

void ILSDestroyRepair::nearestGreedy(std::vector<std::vector<int>> &matrix, std::vector<int> &Cycles, std::vector<bool> &left, int notAssignedCount)
{
    std::array cycle{std::vector<int>(Cycles.begin(), Cycles.begin() + Cycles.size() / 2), std::vector<int>(Cycles.begin() + Cycles.size() / 2, Cycles.end())};
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

    std::vector<int> Temp = cycle[0];
	Temp.insert(Temp.end(), cycle[1].begin(), cycle[1].end());

    Cycles = Temp;
}

void ILSDestroyRepair::destroyRepairPerturbation(std::vector<std::vector<int>> &Matrix, std::vector<int> &Cycles, float deleteRatio)
{
    std::array CyclesArr{std::vector<int>(Cycles.begin(), Cycles.begin() + Cycles.size() / 2), std::vector<int>(Cycles.begin() + Cycles.size() / 2, Cycles.end())};
    auto &CycleA = CyclesArr[0];
    auto &CycleB = CyclesArr[1];

    int deleteQuantity = (int)(Cycles.size()*deleteRatio);
    std::vector<bool> left (Cycles.size(), false);
    int toDeleteIx;
    int toDelete;

    srand(time(NULL));

    for (int i = 0; i < deleteQuantity; i++)
    {

        if (i%2 == 0)
        {   
            toDeleteIx = rand()%CycleA.size();
            toDelete = CycleA[toDeleteIx];
            CycleA.erase(CycleA.begin()+toDeleteIx);

        }
        else
        {
            toDeleteIx = rand()%CycleB.size();
            toDelete = CycleB[toDeleteIx];
            CycleB.erase(CycleB.begin()+toDeleteIx);

        }

        left[toDelete] = true;
    }

    std::vector<int> Temp = CyclesArr[0];
	Temp.insert(Temp.end(), CyclesArr[1].begin(), CyclesArr[1].end());

	Cycles = Temp;

    this->nearestGreedy(Matrix, Cycles, left, deleteQuantity);
}

int ILSDestroyRepair::cyclesLength(std::vector<std::vector<int>> &Matrix, std::vector<int> &Cycles)
{
    int Length = 0;

	for (std::size_t i = 0; i < Cycles.size() / 2 - 1; i++)
		Length += Matrix[Cycles[i]][Cycles[i + 1]];

	Length += Matrix[Cycles[Cycles.size() / 2 - 1]][Cycles[0]];

	for (std::size_t i = Cycles.size() / 2; i < Cycles.size() - 1; i++)
		Length += Matrix[Cycles[i]][Cycles[i + 1]];

	Length += Matrix[Cycles[Cycles.size() - 1]][Cycles[Cycles.size() / 2]];

	return Length;
}

std::vector<int> ILSDestroyRepair::ils(std::vector<std::vector<int>> matrix, std::vector<int> cycles, double timeS)
{
    std::vector<int> x = cycles;
    std::vector<int> y = cycles;

    edgeSteep(matrix, x);

    auto TimeBefore = std::chrono::high_resolution_clock::now();
    
    while (std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - TimeBefore).count() <= timeS)
    {
        y = x;

        destroyRepairPerturbation(matrix, y);
        edgeSteep(matrix, y);

        if (cyclesLength(matrix, x) > cyclesLength(matrix, y))
        {
            std::cout << "UPDATE" << "\n";
            x = y;
        }
    }

    return x;
}





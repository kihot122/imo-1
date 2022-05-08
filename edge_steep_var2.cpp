#include <algorithm>
#include <set>
#include <vector>
#include "main.hpp"
#include <stdlib.h>
#include <time.h>

// // not tested
// std::vector<int> getClosests(int from_index, std::vector<std::vector<int>> &Matrix, int count)
// {

//     std::vector<int> clasestsIndexes(count, UINT16_MAX);
//     bool replaced = true;
//     int maxElementIndex;

//     for (int i; i < Matrix[from_index].size(); i++)
//     {
//         if (replaced)
//         {
//             maxElementIndex = std::max_element(clasestsIndexes.begin(),clasestsIndexes.end()) - clasestsIndexes.begin();
//             replaced = false;
//         }

//         if (Matrix[from_index][i] < clasestsIndexes[maxElementIndex])
//         {
//             clasestsIndexes[maxElementIndex] = Matrix[from_index][i];
//             replaced = true;
//         }
//     }

//     return clasestsIndexes;
// }

int preInCycle(int n, int size)
{
	return n - 1 < 0 ? size - 1 : n - 1;
}

int postInCycle(int n, int size)
{
	return n + 1 == size ? 0 : n + 1;
}

int localMoveDelta(std::vector<std::vector<int>> &Matrix, std::vector<int> &Cycle, int A, int B)
{
	int deltaRet;

	int Size = static_cast<int>(Cycle.size());

	int AMax = A + 1 == Size ? 0 : A + 1;
	int BMax = B + 1 == Size ? 0 : B + 1;

	//(A to B) + (A+1 to B+1) - (A to A+1) - (B + B+1)
	return Matrix[Cycle[A]][Cycle[B]] + Matrix[Cycle[AMax]][Cycle[BMax]] - Matrix[Cycle[A]][Cycle[AMax]] - Matrix[Cycle[B]][Cycle[BMax]];
}

int crossMoveDelta(std::vector<std::vector<int>> &Matrix, std::vector<int> &CycleA, std::vector<int> &CycleB, int A, int B)
{
	int SizeA = static_cast<int>(CycleA.size());
	int SizeB = static_cast<int>(CycleB.size());

	int AMin = A - 1 < 0 ? SizeA - 1 : A - 1;
	int BMin = B - 1 < 0 ? SizeB - 1 : B - 1;

	int AMax = A + 1 == SizeA ? 0 : A + 1;
	int BMax = B + 1 == SizeB ? 0 : B + 1;

	//(A-1 to B) + (A+1 to B) + (B-1 to A) + (B+1 to A)
	//- (A-1 to A) - (A+1 to A) - (B-1 to B) - (B+1 to B)
	return Matrix[CycleA[AMin]][CycleB[B]] + Matrix[CycleA[AMax]][CycleB[B]] + Matrix[CycleB[BMin]][CycleA[A]] + Matrix[CycleB[BMax]][CycleA[A]] - Matrix[CycleA[AMin]][CycleA[A]] - Matrix[CycleA[AMax]][CycleA[A]] - Matrix[CycleB[BMin]][CycleB[B]] -
		   Matrix[CycleB[BMax]][CycleB[B]];
}

enum class ExchangeType2
{
	LOCAL_A,
	LOCAL_B,
	CROSS
};

std::vector<int> EdgeSteepVar2(std::vector<std::vector<int>> Matrix, std::vector<int> Cycles, int candidatesCount)
{
	std::vector<std::vector<int>> candidates;
	candidates.resize(Matrix.size(), std::vector<int>(candidatesCount));

	auto CycleA = std::vector<int>(Cycles.begin(), Cycles.begin() + Cycles.size() / 2);
	auto CycleB = std::vector<int>(Cycles.begin() + Cycles.size() / 2, Cycles.end());

	bool replaced = true;
	int maxElementIndex;

	srand(time(NULL));

	for (int i = 0; i < Matrix.size() - 1; i++)
	{
		for (int j = i + 1; j < Matrix[i].size(); j++)
		{
			if (replaced)
			{
				maxElementIndex = std::max_element(candidates[i].begin(), candidates[i].end()) - candidates[i].begin();
				replaced = false;
			}

			if (Matrix[i][j] < candidates[i][maxElementIndex])
			{
				candidates[i][maxElementIndex] = Matrix[i][j];
				replaced = true;
			}
		}
	}

	bool firstInCycleA;
	int firstInCycleIndex;
	bool secondInCycleA;
	int secondInCycleIndex;

	int minDelta = 0;
	int locDelta = 0, locDeltaOther = 0;
	ExchangeType2 minMoveType;
	ExchangeType2 locMoveType;
	int minChoiceA, minChoiceB;
	int locChoiceA, locChoiceB;
	int preA, preB, postA, postB;
	std::vector<int> iter;

	while (true)
	{
		for (int i = 0; i < candidates.size(); i++)
		{
			firstInCycleIndex = std::find(CycleA.begin(), CycleA.end(), i) - CycleA.begin();
			if (firstInCycleIndex == CycleA.size())
			{
				firstInCycleIndex = std::find(CycleB.begin(), CycleB.end(), i) - CycleB.begin();
				firstInCycleA = false;
			}
			else
			{
				firstInCycleA = true;
			}

			for (int j = 0; j < candidates[i].size(); j++)
			{
				secondInCycleIndex = std::find(CycleA.begin(), CycleA.end(), candidates[i][j]) - CycleA.begin();
				if (secondInCycleIndex == CycleA.size())
				{
					secondInCycleIndex = std::find(CycleB.begin(), CycleB.end(), candidates[i][j]) - CycleB.begin();
					secondInCycleA = false;
				}
				else
				{
					secondInCycleA = true;
				}

				if (secondInCycleA && firstInCycleA)
				{
					locDelta = localMoveDelta(Matrix, CycleA, firstInCycleIndex, secondInCycleIndex);
					locChoiceA = preInCycle(firstInCycleIndex, CycleA.size());
					locChoiceB = preInCycle(secondInCycleIndex, CycleA.size());
					locDeltaOther = localMoveDelta(Matrix, CycleA, locChoiceA, locChoiceB);
					if (locDelta > locDeltaOther)
					{
						locDelta = locDeltaOther;
					}
					else
					{
						locChoiceA = firstInCycleIndex;
						locChoiceB = secondInCycleIndex;
					}

					locMoveType = ExchangeType2::LOCAL_A;
				}
				else if (not(secondInCycleA) && not(firstInCycleA))
				{
					locDelta = localMoveDelta(Matrix, CycleB, firstInCycleIndex, secondInCycleIndex);
					preA = preInCycle(firstInCycleIndex, CycleB.size());
					preB = preInCycle(secondInCycleIndex, CycleB.size());
					locDeltaOther = localMoveDelta(Matrix, CycleB, preA, preB);
					if (locDelta > locDeltaOther)
					{
						locDelta = locDeltaOther;
						locChoiceA = preA;
						locChoiceB = preB;
					}
					else
					{
						locChoiceA = firstInCycleIndex;
						locChoiceB = secondInCycleIndex;
					}

					locMoveType = ExchangeType2::LOCAL_B;
				}
				else
				{
					if (firstInCycleA)
					{
						preA = preInCycle(firstInCycleIndex, CycleA.size());
						postA = postInCycle(firstInCycleIndex, CycleA.size());
						preB = preInCycle(secondInCycleIndex, CycleB.size());
						postB = postInCycle(secondInCycleIndex, CycleB.size());
					}
					else
					{
						preA = preInCycle(firstInCycleIndex, CycleB.size());
						postA = postInCycle(firstInCycleIndex, CycleB.size());
						preB = preInCycle(secondInCycleIndex, CycleA.size());
						postB = postInCycle(secondInCycleIndex, CycleA.size());
					}

					iter = {firstInCycleIndex, preB, firstInCycleIndex, postB, preA, secondInCycleIndex, preA, secondInCycleIndex};
					for (int iteri = 0; iteri < iter.size(); iteri += 2)
					{
						locDeltaOther = crossMoveDelta(Matrix, CycleA, CycleB, iter[iteri], iter[iteri + 1]);

						if (locDelta > locDeltaOther)
						{
							locDelta = locDeltaOther;
							locChoiceA = iter[iteri];
							locChoiceB = iter[iteri + 1];
						}
					}

					locMoveType = ExchangeType2::CROSS;
				}

				if (locDelta < minDelta)
				{
					minDelta = locDelta;
					minMoveType = locMoveType;
					minChoiceA = locChoiceA;
					minChoiceB = locChoiceB;
				}
			}
		}

		if (minDelta < 0)
		{
			switch (minMoveType)
			{
				case ExchangeType2::LOCAL_A:
				{
					std::reverse(CycleA.begin() + minChoiceA + 1, CycleA.begin() + minChoiceB + 1);
					break;
				}
				case ExchangeType2::LOCAL_B:
				{
					std::reverse(CycleB.begin() + minChoiceA + 1, CycleB.begin() + minChoiceB + 1);
					break;
				}
				case ExchangeType2::CROSS:
				{
					std::swap(CycleA[minChoiceA], CycleB[minChoiceB]);
					break;
				}
			}
		}
		else
		{
			break;
		}
	}

	std::vector<int> ret = CycleA;
	ret.insert(ret.end(), CycleB.begin(), CycleB.end());
	return ret;
}
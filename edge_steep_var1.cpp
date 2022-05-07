#include <algorithm>
#include <vector>
#include "main.hpp"

enum class ExchangeType : bool
{
	LOCAL,
	CROSS,
};

struct Exchange
{
	int ChoiceA, ChoiceB;
	int CycleIndex;
	int Delta;
	ExchangeType Type;
};

std::vector<Exchange> Initial(std::vector<std::vector<int>> &Matrix, std::array<std::vector<int>, 2> &CyclesArr)
{
	std::vector<Exchange> BestMoves;

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
				int Delta = Matrix[Cycle[A]][Cycle[B]] + Matrix[Cycle[AMax]][Cycle[BMax]] - Matrix[Cycle[A]][Cycle[AMax]] - Matrix[Cycle[B]][Cycle[BMax]];

				if (Delta < 0)
				{
					BestMoves.push_back(Exchange{
						.ChoiceA = A,
						.ChoiceB = B,
						.CycleIndex = i,
						.Delta = Delta,
						.Type = ExchangeType::LOCAL,
					});
				}
			}
		}
		i++;
	}

	auto &CycleA = CyclesArr[0];
	auto &CycleB = CyclesArr[1];
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
			int Delta = Matrix[CycleA[AMin]][CycleB[B]] + Matrix[CycleA[AMax]][CycleB[B]] + Matrix[CycleB[BMin]][CycleA[A]] + Matrix[CycleB[BMax]][CycleA[A]] - Matrix[CycleA[AMin]][CycleA[A]] - Matrix[CycleA[AMax]][CycleA[A]] -
						Matrix[CycleB[BMin]][CycleB[B]] - Matrix[CycleB[BMax]][CycleB[B]];

			if (Delta < 0)
			{
				BestMoves.push_back(Exchange{
					.ChoiceA = A,
					.ChoiceB = B,
					.Delta = Delta,
					.Type = ExchangeType::CROSS,
				});
			}
		}
	}

	std::sort(BestMoves.begin(), BestMoves.end(), [](auto &X, auto &Y) { return X.Delta < Y.Delta; });
	return BestMoves;
}

std::vector<int> EdgeSteepVar1(std::vector<std::vector<int>> Matrix, std::vector<int> Cycles)
{
	std::array CyclesArr = {std::vector<int>(Cycles.begin(), Cycles.begin() + Cycles.size() / 2), std::vector<int>(Cycles.begin() + Cycles.size() / 2, Cycles.end())};
	auto &CycleA = CyclesArr[0];
	auto &CycleB = CyclesArr[1];
	std::vector<Exchange> BestMoves = Initial(Matrix, CyclesArr);

	do
	{

	} while (not BestMoves.empty());

	std::vector<int> Temp = CyclesArr[0];
	Temp.insert(Temp.end(), CyclesArr[1].begin(), CyclesArr[1].end());

	return Temp;
}
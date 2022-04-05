#include "main.hpp"

enum class VertexType : bool
{
	LOCAL,
	CROSS,
};

std::vector<int> VertexSteep(std::vector<std::vector<int>> Matrix, std::vector<int> Cycles)
{
	std::array CyclesArr = {std::vector<int>(Cycles.begin(), Cycles.begin() + Cycles.size() / 2), std::vector<int>(Cycles.begin() + Cycles.size() / 2, Cycles.end())};
	auto &CycleA = CyclesArr[0];
	auto &CycleB = CyclesArr[1];

	int MinDelta;
	do
	{
		MinDelta = 0;
		int Delta;
		int ChoiceA, ChoiceB, CycleIndex;
		VertexType Type;

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

					//(A-1 to B) + (A+1 to B) + (B-1 to A) + (B+1 to A)
					//- (A-1 to A) - (A+1 to A) - (B-1 to B) - (B+1 to B)
					if (AMax != B and BMax != A)
						Delta = Matrix[Cycle[AMin]][Cycle[B]] + Matrix[Cycle[AMax]][Cycle[B]] + Matrix[Cycle[BMin]][Cycle[A]] + Matrix[Cycle[BMax]][Cycle[A]] - Matrix[Cycle[AMin]][Cycle[A]] - Matrix[Cycle[AMax]][Cycle[A]] -
								Matrix[Cycle[BMin]][Cycle[B]] - Matrix[Cycle[BMax]][Cycle[B]];

					//(A-1 to B) + (B+1 to A)
					//- (A-1 to A) - (B+1 to B)
					else if (BMax != A)
						Delta = Matrix[Cycle[AMin]][Cycle[B]] + Matrix[Cycle[BMax]][Cycle[A]] - Matrix[Cycle[AMin]][Cycle[A]] - Matrix[Cycle[BMax]][Cycle[B]];

					//(A to B-1) + (B to A+1)
					//- (A to A+1) - (B to B-1)
					else
						Delta = Matrix[Cycle[A]][Cycle[BMin]] + Matrix[Cycle[B]][Cycle[AMax]] - Matrix[Cycle[A]][Cycle[AMax]] - Matrix[Cycle[B]][Cycle[BMin]];

					if (Delta < MinDelta)
					{
						MinDelta = Delta;
						ChoiceA = A;
						ChoiceB = B;
						CycleIndex = i;
						Type = VertexType::LOCAL;
					}
				}
			}
			i++;
		}

		for (int A = 0; A < CycleA.size(); A++)
		{
			for (int B = 0; B < CycleB.size(); B++)
			{
				//(A-1 to B) + (A+1 to B) + (B-1 to A) + (B+1 to A)
				//- (A-1 to A) - (A+1 to A) - (B-1 to B) - (B+1 to B)
				int SizeA = static_cast<int>(CycleA.size());
				int SizeB = static_cast<int>(CycleB.size());

				int AMin = A - 1 < 0 ? SizeA - 1 : A - 1;
				int BMin = B - 1 < 0 ? SizeB - 1 : B - 1;

				int AMax = A + 1 == SizeA ? 0 : A + 1;
				int BMax = B + 1 == SizeB ? 0 : B + 1;

				Delta = Matrix[CycleA[AMin]][CycleB[B]] + Matrix[CycleA[AMax]][CycleB[B]] + Matrix[CycleB[BMin]][CycleA[A]] + Matrix[CycleB[BMax]][CycleA[A]] - Matrix[CycleA[AMin]][CycleA[A]] - Matrix[CycleA[AMax]][CycleA[A]] -
						Matrix[CycleB[BMin]][CycleB[B]] - Matrix[CycleB[BMax]][CycleB[B]];

				if (Delta < MinDelta)
				{
					MinDelta = Delta;
					ChoiceA = A;
					ChoiceB = B;
					Type = VertexType::CROSS;
				}
			}
		}

		if (MinDelta < 0)
		{
			switch (Type)
			{
				case VertexType::LOCAL:
				{
					std::swap(CyclesArr[CycleIndex][ChoiceA], CyclesArr[CycleIndex][ChoiceB]);
					break;
				}
				case VertexType::CROSS:
				{
					std::swap(CycleA[ChoiceA], CycleB[ChoiceB]);
					break;
				}
			}
		}

	} while (MinDelta < 0);

	std::vector<int> Temp = CyclesArr[0];
	Temp.insert(Temp.end(), CyclesArr[1].begin(), CyclesArr[1].end());

	return Temp;
}

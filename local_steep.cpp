#include <algorithm>
#include "main.hpp"

enum class EType : bool
{
	VERTEX,
	EDGE,
};

std::vector<int> LocalSteep(std::vector<std::vector<int>> Matrix, std::vector<int> Cycles)
{
	std::array CyclesArr = {std::vector<int>(Cycles.begin(), Cycles.begin() + Cycles.size() / 2), std::vector<int>(Cycles.begin() + Cycles.size() / 2, Cycles.end())};
	int MinDelta;
	bool Flag;

	do
	{
		MinDelta = 0;
		Flag = false;
		int ChoiceA, ChoiceB;
		int Delta;
		EType Type;

		for (auto &&Cycle : CyclesArr)
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
						Type = EType::VERTEX;
					}

					//(A to B) + (A+1 to B+1) - (A to B+1) - (A + B+1)
					Delta = Matrix[Cycle[A]][Cycle[B]] + Matrix[Cycle[AMax]][Cycle[BMax]] - Matrix[Cycle[A]][Cycle[AMax]] - Matrix[Cycle[B]][Cycle[BMax]];

					if (Delta < MinDelta)
					{
						MinDelta = Delta;
						ChoiceA = A;
						ChoiceB = B;
						Type = EType::EDGE;
					}
				}
			}

			if (MinDelta < 0)
			{
				Flag = true;
				switch (Type)
				{
					case EType::VERTEX:
					{
						std::swap(Cycle[ChoiceA], Cycle[ChoiceB]);
						break;
					}
					case EType::EDGE:
					{
						std::reverse(Cycle.begin() + ChoiceA + 1, Cycle.begin() + ChoiceB + 1);
						break;
					}
				}
			}
		}

	} while (Flag);

	std::vector<int> Temp = CyclesArr[0];
	Temp.insert(Temp.end(), CyclesArr[1].begin(), CyclesArr[1].end());
	return Temp;
}
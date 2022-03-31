#include <algorithm>
#include "main.hpp"

enum class EType : bool
{
	VERTEX,
	EDGE,
};

std::vector<int> LocalSteep(std::vector<std::vector<int>> Matrix, std::vector<int> Cycles)
{
	auto CycleA = std::vector<int>(Cycles.begin(), Cycles.begin() + Cycles.size() / 2);
	auto CycleB = std::vector<int>(Cycles.begin() + Cycles.size() / 2, Cycles.end());

	int MinDeltaA, MinDeltaB;
	do
	{
		MinDeltaA = 0;
		MinDeltaB = 0;
		int ChoiceA1, ChoiceA2, ChoiceB1, ChoiceB2;
		int DeltaA, DeltaB;
		EType Type;

		for (int A1 = 0; A1 < CycleA.size(); A1++)
		{
			for (int A2 = A1 + 1; A2 < CycleA.size(); A2++)
			{
				int Size = static_cast<int>(CycleA.size());

				int A1Min = A1 - 1 < 0 ? Size - 1 : A1 - 1;
				int A2Min = A2 - 1 < 0 ? Size - 1 : A2 - 1;

				int A1Max = A1 + 1 == Size ? 0 : A1 + 1;
				int A2Max = A2 + 1 == Size ? 0 : A2 + 1;

				//(A1-1 to A2) + (A1+1 to A2) + (A2-1 to A1) + (A2+1 to A1)
				//- (A1-1 to A1) - (A1+1 to A1) - (A2-1 to A2) - (A2+1 to A2)
				if (A1Max != A2 and A2Max != A1)
					DeltaA = Matrix[CycleA[A1Min]][CycleA[A2]] + Matrix[CycleA[A1Max]][CycleA[A2]] + Matrix[CycleA[A2Min]][CycleA[A1]] + Matrix[CycleA[A2Max]][CycleA[A1]] - Matrix[CycleA[A1Min]][CycleA[A1]] - Matrix[CycleA[A1Max]][CycleA[A1]] -
							 Matrix[CycleA[A2Min]][CycleA[A2]] - Matrix[CycleA[A2Max]][CycleA[A2]];

				//(A1-1 to A2) + (A2+1 to A1)
				//- (A1-1 to A1) - (A2+1 to A2)
				else if (A2Max != A1)
					DeltaA = Matrix[CycleA[A1Min]][CycleA[A2]] + Matrix[CycleA[A2Max]][CycleA[A1]] - Matrix[CycleA[A1Min]][CycleA[A1]] - Matrix[CycleA[A2Max]][CycleA[A2]];

				//(A1 to A2-1) + (A2 to A1+1)
				//- (A1 to A1+1) - (A2 to A2-1)
				else
					DeltaA = Matrix[CycleA[A1]][CycleA[A2Min]] + Matrix[CycleA[A2]][CycleA[A1Max]] - Matrix[CycleA[A1]][CycleA[A1Max]] - Matrix[CycleA[A2]][CycleA[A2Min]];

				if (DeltaA < MinDeltaA)
				{
					MinDeltaA = DeltaA;
					ChoiceA1 = A1;
					ChoiceA2 = A2;
					Type = EType::VERTEX;
				}

				//(X to Y) + (X+1 to Y+1) - (X to X+1) - (Y + Y+1)
				DeltaA = Matrix[CycleA[A1]][CycleA[A2]] + Matrix[CycleA[A1Max]][CycleA[A2Max]] - Matrix[CycleA[A1]][CycleA[A1Max]] - Matrix[CycleA[A2]][CycleA[A2Max]];

				if (DeltaA < MinDeltaA)
				{
					MinDeltaA = DeltaA;
					ChoiceA1 = A1;
					ChoiceA2 = A2;
					Type = EType::EDGE;
				}
			}
		}

		if (MinDeltaA < 0)
		{
			switch (Type)
			{
				case EType::VERTEX:
				{
					std::swap(CycleA[ChoiceA1], CycleA[ChoiceA2]);
					break;
				}
				case EType::EDGE:
				{
					std::reverse(CycleA.begin() + ChoiceA1 + 1, CycleA.begin() + ChoiceA2 + 1);
					break;
				}
			}
		}

		for (int B1 = 0; B1 < CycleB.size(); B1++)
		{
			for (int B2 = B1 + 1; B2 < CycleB.size(); B2++)
			{
				int Size = static_cast<int>(CycleB.size());

				int B1Min = B1 - 1 < 0 ? Size - 1 : B1 - 1;
				int B2Min = B2 - 1 < 0 ? Size - 1 : B2 - 1;

				int B1Max = B1 + 1 == Size ? 0 : B1 + 1;
				int B2Max = B2 + 1 == Size ? 0 : B2 + 1;

				//(B1-1 to B2) + (B1+1 to B2) + (B2-1 to B1) + (B2+1 to B1)
				//- (B1-1 to B1) - (B1+1 to B1) - (B2-1 to B2) - (B2+1 to B2)
				if (B1Max != B2 and B2Max != B1)
					DeltaB = Matrix[CycleB[B1Min]][CycleB[B2]] + Matrix[CycleB[B1Max]][CycleB[B2]] + Matrix[CycleB[B2Min]][CycleB[B1]] + Matrix[CycleB[B2Max]][CycleB[B1]] - Matrix[CycleB[B1Min]][CycleB[B1]] - Matrix[CycleB[B1Max]][CycleB[B1]] -
							 Matrix[CycleB[B2Min]][CycleB[B2]] - Matrix[CycleB[B2Max]][CycleB[B2]];

				//(B1-1 to B2) + (B2+1 to B1)
				//- (B1-1 to B1) - (B2+1 to B2)
				else if (B2Max != B1)
					DeltaB = Matrix[CycleB[B1Min]][CycleB[B2]] + Matrix[CycleB[B2Max]][CycleB[B1]] - Matrix[CycleB[B1Min]][CycleB[B1]] - Matrix[CycleB[B2Max]][CycleB[B2]];

				//(B1 to B2-1) + (B2 to B1+1)
				//- (B1 to B1+1) - (B2 to B2-1)
				else
					DeltaB = Matrix[CycleB[B1]][CycleB[B2Min]] + Matrix[CycleB[B2]][CycleB[B1Max]] - Matrix[CycleB[B1]][CycleB[B1Max]] - Matrix[CycleB[B2]][CycleB[B2Min]];

				if (DeltaB < MinDeltaB)
				{
					MinDeltaB = DeltaB;
					ChoiceB1 = B1;
					ChoiceB2 = B2;
					Type = EType::VERTEX;
				}

				DeltaB = Matrix[CycleB[B1]][CycleB[B2]] + Matrix[CycleB[B1Max]][CycleB[B2Max]] - Matrix[CycleB[B1]][CycleB[B1Max]] - Matrix[CycleB[B2]][CycleB[B2Max]];

				if (DeltaB < MinDeltaB)
				{
					MinDeltaB = DeltaB;
					ChoiceB1 = B1;
					ChoiceB2 = B2;
					Type = EType::EDGE;
				}
			}
		}

		if (MinDeltaB < 0)
		{
			switch (Type)
			{
				case EType::VERTEX:
				{
					std::swap(CycleB[ChoiceB1], CycleB[ChoiceB2]);
					break;
				}
				case EType::EDGE:
				{
					std::reverse(CycleB.begin() + ChoiceB1 + 1, CycleB.begin() + ChoiceB2 + 1);
					break;
				}
			}
		}

	} while (MinDeltaA < 0 or MinDeltaB < 0);

	CycleA.insert(CycleA.end(), CycleB.begin(), CycleB.end());
	return std::vector<int>(CycleA.begin(), CycleA.end());
}
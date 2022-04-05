#include "main.hpp"

std::vector<int> CrossSteep(std::vector<std::vector<int>> Matrix, std::vector<int> Cycles)
{
	auto CycleA = std::vector<int>(Cycles.begin(), Cycles.begin() + Cycles.size() / 2);
	auto CycleB = std::vector<int>(Cycles.begin() + Cycles.size() / 2, Cycles.end());

	int MinDelta;
	do
	{
		MinDelta = 0;
		int ChoiceA, ChoiceB;
		int Delta;
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
				}
			}
		}
		if (MinDelta < 0)
			std::swap(CycleA[ChoiceA], CycleB[ChoiceB]);

	} while (MinDelta < 0);

	CycleA.insert(CycleA.end(), CycleB.begin(), CycleB.end());
	return std::vector<int>(CycleA.begin(), CycleA.end());
}
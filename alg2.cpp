#include <algorithm>
#include <deque>
#include <random>
#include <set>
#include "main.hpp"

std::vector<int> Alg2(std::vector<std::vector<int>> Matrix, int StartNode)
{
	std::random_device Rand;

	std::default_random_engine Engine(Rand());
	std::uniform_int_distribution Dist((size_t)0, Matrix.size() - 1);

	std::set<int> AvailableNodes;
	for (int i = 0; i < Matrix.size(); i++)
		AvailableNodes.insert(i);

	int PivotA = StartNode == -1 ? Dist(Engine) : StartNode;
	int PivotB = std::distance(Matrix[PivotA].begin(), std::max_element(Matrix[PivotA].begin(), Matrix[PivotA].end()));

	for (auto &Row : Matrix)
		for (auto &Col : Row)
			if (Col == 0)
				Col = 0x0fffffff;

	int SecondA = std::distance(Matrix[PivotA].begin(), std::min_element(Matrix[PivotA].begin(), Matrix[PivotA].end()));
	int SecondB = std::distance(Matrix[PivotB].begin(), std::min_element(Matrix[PivotB].begin(), Matrix[PivotB].end()));

	std::deque<int> CycleA{PivotA, SecondA}, CycleB{PivotB, SecondB};
	for (auto Index : {PivotA, SecondA, PivotB, SecondB})
		AvailableNodes.erase(Index);

	bool Flag = true;
	while (not AvailableNodes.empty())
	{
		int BestDelta = 0x0fffffff;
		int BestNode = -1;
		int BestPlace = -1;
		int Delta = 0;

		if (Flag)
		{
			for (size_t i = 0; i < CycleA.size(); i++)
			{
				for (auto Candidate : AvailableNodes)
				{
					Delta = Matrix[CycleA[i]][Candidate] + Matrix[Candidate][CycleA[(i + 1) % CycleA.size()]] - Matrix[CycleA[i]][CycleA[(i + 1) % CycleA.size()]];
					if (Delta < BestDelta)
					{
						BestDelta = Delta;
						BestNode = Candidate;
						BestPlace = i;
					}
				}
			}
			CycleA.insert(CycleA.begin() + BestPlace + 1, BestNode);
		}
		else
		{
			for (size_t i = 0; i < CycleB.size(); i++)
			{
				for (auto Candidate : AvailableNodes)
				{
					Delta = Matrix[CycleB[i]][Candidate] + Matrix[Candidate][CycleB[(i + 1) % CycleB.size()]] - Matrix[CycleB[i]][CycleB[(i + 1) % CycleB.size()]];
					if (Delta < BestDelta)
					{
						BestDelta = Delta;
						BestNode = Candidate;
						BestPlace = i;
					}
				}
			}
			CycleB.insert(CycleB.begin() + BestPlace + 1, BestNode);
		}

		AvailableNodes.erase(BestNode);
		Flag = not Flag;
	}

	CycleA.insert(CycleA.end(), CycleB.begin(), CycleB.end());
	return std::vector<int>(CycleA.begin(), CycleA.end());
}
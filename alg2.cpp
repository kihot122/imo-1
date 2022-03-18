#include <algorithm>
#include <random>
#include <set>
#include "main.hpp"

std::vector<int> Alg2(std::vector<std::vector<int>> Matrix)
{
	std::random_device Rand;

	std::default_random_engine Engine(Rand());
	std::uniform_int_distribution<> Dist(0, Matrix.size() - 1);

	std::set<int> AvailableNodes;
	for (int i = 0; i < Matrix.size(); i++)
		AvailableNodes.insert(i);

	int PivotA = Dist(Engine);
	int PivotB = std::distance(Matrix[PivotA].begin(), std::max_element(Matrix[PivotA].begin(), Matrix[PivotA].end()));
	int SecondA = std::distance(Matrix[PivotA].begin(), std::min_element(Matrix[PivotA].begin(), Matrix[PivotA].end(), [&](auto &a, auto &b) { return a == 0 ? false : a < b; }));
	int SecondB = std::distance(Matrix[PivotB].begin(), std::min_element(Matrix[PivotB].begin(), Matrix[PivotB].end(), [&](auto &a, auto &b) { return a == 0 ? false : a < b; }));

	std::vector<int> CycleA{PivotA, SecondA}, CycleB{PivotB, SecondB};
	for (auto Index : {PivotA, SecondA, PivotB, SecondB})
		AvailableNodes.erase(Index);

	bool Flag = true;
	while (not AvailableNodes.empty())
	{
		int Best = 0x0fffffff;
		int BestIndex = -1;
		int Delta = 0;

		if (Flag)
		{
			for (size_t i = 0; i < CycleA.size(); i++)
			{
				for (auto Candidate : AvailableNodes)
				{
					Delta = Matrix[CycleA[i]][Candidate] + Matrix[Candidate][CycleA[(i + 1) % CycleA.size()]] - Matrix[CycleA[i]][CycleA[(i + 1) % CycleA.size()]];
					if (Delta < Best)
					{
						Best = Delta;
						BestIndex = Candidate;
					}
				}
			}
			CycleA.push_back(BestIndex);
		}
		else
		{
			for (size_t i = 0; i < CycleB.size(); i++)
			{
				for (auto Candidate : AvailableNodes)
				{
					Delta = Matrix[CycleB[i]][Candidate] + Matrix[Candidate][CycleB[(i + 1) % CycleB.size()]] - Matrix[CycleB[i]][CycleB[(i + 1) % CycleB.size()]];
					if (Delta < Best)
					{
						Best = Delta;
						BestIndex = Candidate;
					}
				}
			}
			CycleB.push_back(BestIndex);
		}

		AvailableNodes.erase(BestIndex);
		Flag = not Flag;
	}

	CycleA.insert(CycleA.end(), CycleB.begin(), CycleB.end());
	return CycleA;
}
#include <algorithm>
#include <array>
#include <deque>
#include <random>
#include <set>
#include "main.hpp"

std::vector<int> Alg3(std::vector<std::vector<int>> Matrix, int StartNode)
{
	std::random_device Rand;

	std::default_random_engine Engine(Rand());
	std::uniform_int_distribution<> Dist(0, Matrix.size() - 1);

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
		std::vector<std::array<int, 2>> Deltas;
		std::vector<int> Nodes;
		std::vector<int> Places;

		int WorstRegret = -1;
		int BestDelta = 0x0fffffff;
		int BestNode = -1;
		int BestPlace = -1;

		if (Flag)
		{
			for (auto Candidate : AvailableNodes)
			{
				Deltas.push_back({0x0fffffff, 0x0fffffff});
				Nodes.push_back(Candidate);
				Places.push_back(-1);

				auto &BestDeltas = Deltas.back();
				int &BestPlace = Places.back();
				int Delta = 0;

				for (size_t i = 0; i < CycleA.size(); i++)
				{
					Delta = Matrix[CycleA[i]][Candidate] + Matrix[Candidate][CycleA[(i + 1) % CycleA.size()]] - Matrix[CycleA[i]][CycleA[(i + 1) % CycleA.size()]];
					if (Delta < BestDeltas[0])
					{
						BestDeltas[1] = BestDeltas[0];
						BestDeltas[0] = Delta;
						BestPlace = i;
					}
					else if (Delta < BestDeltas[1])
					{
						BestDeltas[1] = Delta;
					}
				}
			}

			for (size_t i = 0; i < Nodes.size(); i++)
			{
				int Regret = Deltas[i][1] - Deltas[i][0];
				if (Regret > WorstRegret or Regret == WorstRegret and Deltas[i][0] < BestDelta)
				{
					WorstRegret = Regret;
					BestDelta = Deltas[i][0];
					BestNode = Nodes[i];
					BestPlace = Places[i];
				}
			}

			CycleA.insert(CycleA.begin() + BestPlace + 1, BestNode);
		}
		else
		{
			for (auto Candidate : AvailableNodes)
			{
				Deltas.push_back({0x0fffffff, 0x0fffffff});
				Nodes.push_back(Candidate);
				Places.push_back(-1);

				auto &BestDeltas = Deltas.back();
				int &BestPlace = Places.back();
				int Delta = 0;

				for (size_t i = 0; i < CycleB.size(); i++)
				{
					Delta = Matrix[CycleB[i]][Candidate] + Matrix[Candidate][CycleB[(i + 1) % CycleB.size()]] - Matrix[CycleB[i]][CycleB[(i + 1) % CycleB.size()]];
					if (Delta < BestDeltas[0])
					{
						BestDeltas[1] = BestDeltas[0];
						BestDeltas[0] = Delta;
						BestPlace = i;
					}
					else if (Delta < BestDeltas[1])
					{
						BestDeltas[1] = Delta;
					}
				}
			}

			for (size_t i = 0; i < Nodes.size(); i++)
			{
				int Regret = Deltas[i][1] - Deltas[i][0];
				if (Regret > WorstRegret or Regret == WorstRegret and Deltas[i][0] < BestDelta)
				{
					WorstRegret = Regret;
					BestDelta = Deltas[i][0];
					BestNode = Nodes[i];
					BestPlace = Places[i];
				}
			}

			CycleB.insert(CycleB.begin() + BestPlace + 1, BestNode);
		}

		Deltas.clear();
		Nodes.clear();
		Places.clear();

		AvailableNodes.erase(BestNode);
		Flag = not Flag;
	}

	CycleA.insert(CycleA.end(), CycleB.begin(), CycleB.end());
	return std::vector<int>(CycleA.begin(), CycleA.end());
}
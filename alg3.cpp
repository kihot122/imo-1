#include <algorithm>
#include <array>
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
	int SecondA = std::distance(Matrix[PivotA].begin(), std::min_element(Matrix[PivotA].begin(), Matrix[PivotA].end(), [&](auto &a, auto &b) { return a == 0 ? false : a < b; }));
	int SecondB = std::distance(Matrix[PivotB].begin(), std::min_element(Matrix[PivotB].begin(), Matrix[PivotB].end(), [&](auto &a, auto &b) { return a == 0 ? false : a < b; }));

	std::vector<int> CycleA{PivotA, SecondA}, CycleB{PivotB, SecondB};
	for (auto Index : {PivotA, SecondA, PivotB, SecondB})
		AvailableNodes.erase(Index);

	bool Flag = true;
	while (not AvailableNodes.empty())
	{
		std::vector<std::array<int, 2>> Deltas;
		std::vector<int> Indices;

		int WorstRegret = -1;
		int BestDelta = 0x0fffffff;
		int SelectedIndex = -1;

		if (Flag)
		{
			for (auto Candidate : AvailableNodes)
			{
				Deltas.push_back({0x0fffffff, 0x0fffffff});
				Indices.push_back(-1);
				auto &Best = Deltas.back();
				int &BestIndex = Indices.back();
				int Delta = 0;

				for (size_t i = 0; i < CycleA.size(); i++)
				{
					Delta = Matrix[CycleA[i]][Candidate] + Matrix[Candidate][CycleA[(i + 1) % CycleA.size()]] - Matrix[CycleA[i]][CycleA[(i + 1) % CycleA.size()]];
					if (Delta < Best[0])
					{
						Best[1] = Best[0];
						Best[0] = Delta;
						BestIndex = Candidate;
					}
					else if (Delta < Best[1])
					{
						Best[1] = Delta;
					}
				}
			}

			for (size_t i = 0; i < Deltas.size(); i++)
			{
				int Regret = Deltas[i][1] - Deltas[i][0];
				if (Regret > WorstRegret)
				{
					WorstRegret = Regret;
					SelectedIndex = Indices[i];
					BestDelta = Deltas[i][0];
				}
				else if (Regret == WorstRegret and Deltas[i][0] < BestDelta)
				{
					BestDelta = Deltas[i][0];
					SelectedIndex = Indices[i];
				}
			}

			CycleA.push_back(SelectedIndex);
		}
		else
		{
			for (auto Candidate : AvailableNodes)
			{
				Deltas.push_back({0x0fffffff, 0x0fffffff});
				Indices.push_back(-1);
				auto &Best = Deltas.back();
				int &BestIndex = Indices.back();
				int Delta = 0;

				for (size_t i = 0; i < CycleB.size(); i++)
				{
					Delta = Matrix[CycleB[i]][Candidate] + Matrix[Candidate][CycleB[(i + 1) % CycleB.size()]] - Matrix[CycleB[i]][CycleB[(i + 1) % CycleB.size()]];
					if (Delta < Best[0])
					{
						Best[1] = Best[0];
						Best[0] = Delta;
						BestIndex = Candidate;
					}
					else if (Delta < Best[1])
					{
						Best[1] = Delta;
					}
				}
			}

			for (size_t i = 0; i < Deltas.size(); i++)
			{
				int Regret = Deltas[i][1] - Deltas[i][0];
				if (Regret > WorstRegret)
				{
					WorstRegret = Regret;
					SelectedIndex = Indices[i];
				}
				else if (Regret == WorstRegret and Deltas[i][0] < BestDelta)
				{
					BestDelta = Deltas[i][0];
					SelectedIndex = Indices[i];
				}
			}

			CycleB.push_back(SelectedIndex);
		}

		Deltas.clear();
		Indices.clear();
		AvailableNodes.erase(SelectedIndex);
		Flag = not Flag;
	}

	CycleA.insert(CycleA.end(), CycleB.begin(), CycleB.end());
	return CycleA;
}
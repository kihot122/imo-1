#include <algorithm>
#include <map>
#include <random>
#include <ranges>
#include "main.hpp"

auto Range(int i)
{
	return std::views::iota(0, i);
}

auto Range(int i, int j)
{
	return std::views::iota(i, j);
}

bool operator==(const VCycle &A, const VCycle &B)
{
	std::map<std::pair<int, int>, int> Edges;

	for (int &&i : Range(A.size()))
	{
		const auto &EdgeA = std::pair{A[i], A[(i + 1) % A.size()]};
		const auto &EdgeB = std::pair{B[i], B[(i + 1) % B.size()]};

		if (Edges.contains(EdgeA))
			Edges[EdgeA]++;
		else
			Edges[EdgeA] = 0;

		if (Edges.contains(EdgeB))
			Edges[EdgeB]++;
		else
			Edges[EdgeB] = 0;
	}

	for (auto &&[Edge, Count] : Edges)
		if (Count == 0)
			return false;

	return true;
}

std::pair<VCycle, std::vector<bool>> operator*(const VCycle &A, const VCycle &B)
{
	std::map<std::pair<int, int>, int> Edges;

	for (int &&i : Range(A.size()))
	{
		const auto &EdgeA = std::pair{A[i], A[(i + 1) % A.size()]};
		const auto &EdgeB = std::pair{B[i], B[(i + 1) % B.size()]};

		if (Edges.contains(EdgeA))
			Edges[EdgeA]++;
		else
			Edges[EdgeA] = 0;

		if (Edges.contains(EdgeB))
			Edges[EdgeB]++;
		else
			Edges[EdgeB] = 0;
	}

	std::vector<bool> Mask(A.size());
	for (int &&i : Range(A.size()))
	{
		Mask[i] = Edges[{A[i], A[(i + 1) % A.size()]}] ? false : true;
	}

	return {A, Mask};
}

void Perturbate(VCycle &Cycle)
{
	static std::random_device Rand;
	static std::default_random_engine Engine(Rand());
	static std::uniform_int_distribution Dist(0, 1);

	static auto View = std::views::iota(0, (int)Cycle.size());
	static auto Range = std::vector<int>(View.begin(), View.end());

	std::shuffle(Range.begin(), Range.end(), Engine);
	// Exchange 3 vertices
	// 1 -> 2, 2 -> 3, 3 -> 1
	if (not Dist(Engine))
	{
		auto Temp = Cycle[Range[0]];
		Cycle[Range[0]] = Cycle[Range[2]];
		Cycle[Range[2]] = Cycle[Range[1]];
		Cycle[Range[1]] = Temp;
	}
	// But sometimes also 5 vertices
	// 1 -> 2, 2 -> 3, 3 -> 4, 4 -> 5, 5 -> 1
	else
	{
		auto Temp = Cycle[Range[0]];
		Cycle[Range[0]] = Cycle[Range[4]];
		Cycle[Range[4]] = Cycle[Range[3]];
		Cycle[Range[3]] = Cycle[Range[2]];
		Cycle[Range[2]] = Cycle[Range[1]];
		Cycle[Range[1]] = Temp;
	}
}

VCycle Genetic(VMat Matrix, bool UseSteep, int CrossPopulationSize, int Epoch, float MutationFactor)
{
	std::random_device Rand;
	std::default_random_engine Engine(Rand());
	std::uniform_real_distribution Dist(0.0f, 1.0f);

	std::vector<std::pair<VCycle, int>> Population;
	int PopulationSize = CrossPopulationSize * (CrossPopulationSize - 1) / 2;
	std::pair<VCycle, int> Best = {VCycle(), 0x0fffffff};

	for (int &&i : Range(PopulationSize))
	{
		Population.push_back({EdgeSteep(Matrix, Alg2(Matrix, i % Matrix.size())), -1});
		Population[i].second = ChainLength(Matrix, Population[i].first);

		auto &Latest = Population[Population.size() - 1].first;
		for (int &&j : Range(Population.size() - 1))
		{
			if (Latest == Population[j].first)
				Perturbate(Latest);
		}
	}

	for (int &&i : Range(Epoch))
	{
		std::sort(Population.begin(), Population.end(), [](auto &A, auto &B) { return A.second < B.second; });
		if (Population.begin()->second < Best.second)
			Best = *Population.begin();

		std::vector<std::pair<VCycle, int>> NewPopulation;
		for (int &&i : Range(CrossPopulationSize))
		{
			for (int &&j : Range(i + 1, CrossPopulationSize))
			{
				auto &&New = Population[i].first * Population[j].first;
				auto cycleA = std::vector<int>(New.first.begin(), New.first.begin() + New.first.size() / 2);
				auto cycleB = std::vector<int>(New.first.begin() + New.first.size() / 2, New.first.end());
				auto maskA = std::vector<bool>(New.second.begin(), New.second.begin() + New.second.size() / 2);
				auto maskB = std::vector<bool>(New.second.begin() + New.second.size() / 2, New.second.end());
				steepRepair(Matrix, cycleA, cycleB, maskA, maskB);

				cycleA.insert(cycleA.end(), cycleB.begin(), cycleB.end());
				New.first = cycleA;

				if (Dist(Engine) < MutationFactor)
				{
					Perturbate(New.first);
				}

				if (UseSteep)
					New.first = EdgeSteep(Matrix, New.first);

				NewPopulation.push_back({New.first, ChainLength(Matrix, New.first)});
			}
		}
	}

	return Best.first;
}
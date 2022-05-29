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
	std::map<std::tuple<int, int>, int> Edges;

	for (int &&i : Range(A.size()))
	{
		const auto &EdgeA = std::tuple{A[i], A[(i + 1) % A.size()]};
		const auto &EdgeB = std::tuple{B[i], B[(i + 1) % B.size()]};

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

std::tuple<VCycle, std::vector<bool>> operator*(const VCycle &A, const VCycle &B)
{
	std::map<std::tuple<int, int>, int> Edges;

	for (int &&i : Range(A.size()))
	{
		const auto &EdgeA = std::tuple{A[i], A[(i + 1) % A.size()]};
		const auto &EdgeB = std::tuple{B[i], B[(i + 1) % B.size()]};

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
		Mask[i] = Edges[std::tuple{A[i], A[(i + 1) % A.size()]}] ? 0 : 1;
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

VCycle Genetic(VMat Matrix, int CrossPopulationSize, int Epoch)
{
	std::vector<std::tuple<VCycle, int>> Population;
	int PopulationSize = CrossPopulationSize * (CrossPopulationSize - 1) / 2;

	for (int &&i : Range(PopulationSize))
	{
		Population.push_back({EdgeSteep(Matrix, Alg2(Matrix, i % Matrix.size())), -1});
		auto &Latest = std::get<0>(Population[Population.size() - 1]);
		for (int &&j : Range(Population.size() - 1))
		{
			if (Latest == std::get<0>(Population[j]))
				Perturbate(Latest);
		}
	}

	for (int &&i : Range(Epoch))
	{
		std::sort(Population.begin(), Population.end(), [](auto &A, auto &B) { return std::get<1>(A) < std::get<1>(B); });

		std::vector<std::tuple<VCycle, int>> NewPopulation;
		for (int &&i : Range(CrossPopulationSize))
		{
			for (int &&j : Range(i + 1, CrossPopulationSize))
			{
				auto &&New = std::get<0>(Population[i]) * std::get<0>(Population[j]);
				// fix shit on mask vector

				NewPopulation.push_back({std::get<0>(New), -1});
			}
		}
	}

	return {};
}
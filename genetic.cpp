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
	for (int i : Range(A.size()))
		if (A[i] != B[i])
			return false;

	return true;
}

void Perturbate()
{}

VCycle Genetic(VMat Matrix, int CrossPopulationSize)
{
	std::vector<VCycle> Population;
	int PopulationSize = CrossPopulationSize * (CrossPopulationSize - 1) / 2;

	for (int &&i : Range(PopulationSize))
	{
		Population.push_back(EdgeSteep(Matrix, Alg2(Matrix, i % Matrix.size())));
		const auto &Latest = Population[Population.size() - 1];
		for (int &&j : Range(Population.size() - 1))
		{
			if (Latest == Population[j])
			{}
		}
	}
}
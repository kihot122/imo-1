#include <algorithm>
#include <random>
#include "main.hpp"

std::vector<int> Alg2(std::vector<std::vector<int>> Matrix)
{
	std::random_device Rand;

	std::default_random_engine Engine(Rand());
	std::uniform_int_distribution<> Dist(0, Matrix.size() - 1);

	int PivotA = Dist(Engine);
	int PivotB = std::distance(Matrix[PivotA].begin(), std::max_element(Matrix[PivotA].begin(), Matrix[PivotA].end()));
	int SecondA = std::distance(Matrix[PivotA].begin(), std::min_element(Matrix[PivotA].begin(), Matrix[PivotA].end(), [&](auto &a, auto &b) { return a == 0 ? false : a < b; }));
	int SecondB = std::distance(Matrix[PivotB].begin(), std::min_element(Matrix[PivotB].begin(), Matrix[PivotB].end(), [&](auto &a, auto &b) { return a == 0 ? false : a < b; }));

	std::vector<int> CycleA{PivotA, SecondA}, CycleB{PivotB, SecondB};

	return {};
}
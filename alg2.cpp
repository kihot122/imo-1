#include <algorithm>
#include <random>
#include "main.hpp"

std::vector<int> Alg2(const std::vector<std::vector<int>> &Matrix)
{
	std::random_device Rand;

	std::default_random_engine Engine(Rand());
	std::uniform_int_distribution<> Dist(0, Matrix.size() - 1);

	auto PivotA = Dist(Engine);
	auto PivotB = std::distance(Matrix[PivotA].begin(), std::max_element(Matrix[PivotA].begin(), Matrix[PivotA].end()));
	auto SecondA = std::distance(Matrix[PivotA].begin(), std::min_element(Matrix[PivotA].begin(), Matrix[PivotA].end(), [&](auto &a, auto &b) { return a == 0 ? false : a < b; }));
	auto SecondB = std::distance(Matrix[PivotB].begin(), std::min_element(Matrix[PivotB].begin(), Matrix[PivotB].end(), [&](auto &a, auto &b) { return a == 0 ? false : a < b; }));

	return {};
}
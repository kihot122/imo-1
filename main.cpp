#include <algorithm>
#include <array>
#include <fstream>
#include <numeric>
#include "main.hpp"

int main(int argc, char **argv)
{
	auto Matrix = ReadMat(argc == 2 ? argv[1] : "kroA100.tsp");
	auto Positions = ReadPos(argc == 2 ? argv[1] : "kroA100.tsp");

	std::array<std::array<int, 8>, 100> Results;
	std::array<int, 8> BestLength;
	std::ranges::fill(BestLength.begin(), BestLength.end(), 0x0fffffff);
	std::array<std::vector<int>, 8> BestChain;

	for (size_t i = 0; i < 100; i++)
	{
		std::array<std::vector<int>, 8> Chains;
		std::array<int, 8> Lengths;

		Chains[0] = std::vector<int>(100); // CrossGreedy with RandomChain here
		Chains[1] = CrossSteep(Matrix, RandomChain(Matrix, 1000));
		Chains[2] = std::vector<int>(100); // LocalGreedy with RandomChain here
		Chains[3] = LocalSteep(Matrix, RandomChain(Matrix, 1000));
		Chains[4] = std::vector<int>(100); // CrossGreedy with Alg2 here
		Chains[5] = CrossSteep(Matrix, Alg2(Matrix, i));
		Chains[6] = std::vector<int>(100); // LocalGreedy with Alg2 here
		Chains[7] = LocalSteep(Matrix, Alg2(Matrix, i));

		for (int j = 0; j < 8; j++)
		{
			Lengths[j] = ChainLength(Chains[j], Matrix);
			if (Lengths[j] < BestLength[j])
			{
				BestLength[j] = Lengths[j];
				BestChain[j] = Chains[j];
			}
			Results[i][j] = Lengths[j];
		}
	}

	std::fstream File("Results.txt", std::ios::out);
	for (int i = 0; i < 8; i++)
	{
		int Max = (*std::max_element(Results.begin(), Results.end(), [&i](auto &a, auto &b) { return a[i] < b[i]; }))[i];
		int Min = (*std::min_element(Results.begin(), Results.end(), [&i](auto &a, auto &b) { return a[i] < b[i]; }))[i];
		int Avg = (std::accumulate(Results.begin(), Results.end(), 0, [&i](auto &&acc, auto &&ref) { return acc + ref[i]; }) / 100);

		WriteChain(BestChain[i], Positions, "Chain-" + std::to_string(i));
		File << Max << " " << Min << " " << Avg << std::endl;
	}
}
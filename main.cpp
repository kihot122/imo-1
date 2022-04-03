#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include "main.hpp"

int main(int argc, char **argv)
{
	auto Matrix = ReadMat(argc == 2 ? argv[1] : "kroA100.tsp");
	auto Positions = ReadPos(argc == 2 ? argv[1] : "kroA100.tsp");

	std::array<std::array<int, 8>, 100> Results;
	std::array<std::array<size_t, 8>, 100> Times;
	std::array<int, 8> BestLength;
	std::ranges::fill(BestLength.begin(), BestLength.end(), 0x0fffffff);
	std::array<std::vector<int>, 8> BestChain;

	auto TimeA = std::chrono::high_resolution_clock::now();
	auto TimeB = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < 100; i++)
	{
		std::array<std::vector<int>, 8> Chains;
		std::array<int, 8> Lengths;

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[0] = CrossGreedy(Matrix, RandomChain(Matrix, 100));
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][0] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[1] = CrossSteep(Matrix, RandomChain(Matrix, 100));
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][1] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[2] = LocalGreedy(Matrix, RandomChain(Matrix, 100));
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][2] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[3] = LocalSteep(Matrix, RandomChain(Matrix, 100));
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][3] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[4] = CrossGreedy(Matrix, Alg2(Matrix, i));
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][4] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[5] = CrossSteep(Matrix, Alg2(Matrix, i));
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][5] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[6] = LocalGreedy(Matrix, Alg2(Matrix, i));
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][6] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[7] = LocalSteep(Matrix, Alg2(Matrix, i));
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][6] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

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

	std::fstream FileResults("Results.txt", std::ios::out);
	std::fstream FileTimes("Times.txt", std::ios::out);
	for (int i = 0; i < 8; i++)
	{
		int MaxLength = (*std::max_element(Results.begin(), Results.end(), [&i](auto &a, auto &b) { return a[i] < b[i]; }))[i];
		int MinLength = (*std::min_element(Results.begin(), Results.end(), [&i](auto &a, auto &b) { return a[i] < b[i]; }))[i];
		int AvgLength = (std::accumulate(Results.begin(), Results.end(), 0, [&i](auto &&acc, auto &&ref) { return acc + ref[i]; }) / 100);

		size_t MaxTime = (*std::max_element(Times.begin(), Times.end(), [&i](auto &a, auto &b) { return a[i] < b[i]; }))[i];
		size_t MinTime = (*std::min_element(Times.begin(), Times.end(), [&i](auto &a, auto &b) { return a[i] < b[i]; }))[i];
		size_t AvgTime = (std::accumulate(Times.begin(), Times.end(), 0, [&i](auto &&acc, auto &&ref) { return acc + ref[i]; }) / 100);

		WriteChain(BestChain[i], Positions, "Chain-" + std::to_string(i));
		FileResults << MaxLength << " " << MinLength << " " << AvgLength << std::endl;
		FileTimes << MaxTime << " " << MinTime << " " << AvgTime << std::endl;
	}
}
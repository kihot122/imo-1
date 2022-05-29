#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include "main.hpp"

int main(int argc, char **argv)
{
	auto Matrix = ReadMat(argc == 2 ? argv[1] : "kroA200.tsp");
	auto Positions = ReadPos(argc == 2 ? argv[1] : "kroA200.tsp");
	const int ChainNum = 3;
	const int Tries = 10;

	//auto res = Genetic(Matrix);
	//auto len = ChainLength(Matrix, res);

	std::array<std::array<int, ChainNum>, Tries> Results;
	std::array<std::array<size_t, ChainNum>, Tries> Times;
	std::array<int, ChainNum> BestLength;
	std::ranges::fill(BestLength.begin(), BestLength.end(), 0x0fffffff);
	std::array<std::vector<int>, ChainNum> BestChain;

	auto TimeA = std::chrono::high_resolution_clock::now();
	auto TimeB = std::chrono::high_resolution_clock::now();

	ILSDestroyRepair ILSBD = ILSDestroyRepair();

	for (size_t i : std::views::iota(0, Tries))
	{
		std::array<std::vector<int>, ChainNum> Chains;
		std::array<int, ChainNum> Lengths;

		auto Initial = Alg2(Matrix);

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[0] = msls(Matrix);
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][0] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[1] = EdgeSteepIteratedVar1(Matrix, Initial, 1500);
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][1] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[2] = ILSBD.ils(Matrix, Initial, 0.6, 1000000);
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][2] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		for (size_t j : std::views::iota(0, ChainNum))
		{
			Lengths[j] = ChainLength(Matrix, Chains[j]);
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
	for (size_t i : std::views::iota(0, ChainNum))
	{
		int MaxLength = (*std::max_element(Results.begin(), Results.end(), [&i](auto &a, auto &b) { return a[i] < b[i]; }))[i];
		int MinLength = (*std::min_element(Results.begin(), Results.end(), [&i](auto &a, auto &b) { return a[i] < b[i]; }))[i];
		int AvgLength = (std::accumulate(Results.begin(), Results.end(), 0, [&i](auto &&acc, auto &&ref) { return acc + ref[i]; }) / Tries);

		size_t MaxTime = (*std::max_element(Times.begin(), Times.end(), [&i](auto &a, auto &b) { return a[i] < b[i]; }))[i];
		size_t MinTime = (*std::min_element(Times.begin(), Times.end(), [&i](auto &a, auto &b) { return a[i] < b[i]; }))[i];
		size_t AvgTime = (std::accumulate(Times.begin(), Times.end(), (size_t)0, [&i](auto &&acc, auto &&ref) { return acc + ref[i]; }) / Tries);

		WriteChain(BestChain[i], Positions, "Chain-" + std::to_string(i));
		FileResults << MaxLength << " " << MinLength << " " << AvgLength << std::endl;
		FileTimes << MaxTime << " " << MinTime << " " << AvgTime << std::endl;
	}
}
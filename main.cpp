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

	std::array<std::array<int, 12>, 100> Results;
	std::array<std::array<size_t, 12>, 100> Times;
	std::array<int, 12> BestLength;
	std::ranges::fill(BestLength.begin(), BestLength.end(), 0x0fffffff);
	std::array<std::vector<int>, 12> BestChain;

	auto TimeA = std::chrono::high_resolution_clock::now();
	auto TimeB = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < 100; i++)
	{
		std::array<std::vector<int>, 12> Chains;
		std::array<int, 12> Lengths;

		auto Initial = RandomChain(Matrix, 100);

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[0] = VertexRandom(Matrix, Initial, 400000);
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][0] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[1] = VertexGreedy(Matrix, Initial);
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][1] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[2] = VertexSteep(Matrix, Initial);
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][2] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[3] = EdgeRandom(Matrix, Initial, 400000);
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][3] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[4] = EdgeGreedy(Matrix, Initial);
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][4] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[5] = EdgeSteep(Matrix, Initial);
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][5] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		Initial = Alg2(Matrix, i);

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[6] = VertexRandom(Matrix, Initial, 400000);
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][6] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[7] = VertexGreedy(Matrix, Initial);
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][7] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[8] = VertexSteep(Matrix, Initial);
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][8] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[9] = EdgeRandom(Matrix, Initial, 400000);
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][9] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[10] = EdgeGreedy(Matrix, Initial);
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][10] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		TimeA = std::chrono::high_resolution_clock::now();
		Chains[11] = EdgeSteep(Matrix, Initial);
		TimeB = std::chrono::high_resolution_clock::now();
		Times[i][11] = std::chrono::duration<size_t, std::nano>(TimeB - TimeA).count();

		for (int j = 0; j < 12; j++)
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
	for (int i = 0; i < 12; i++)
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
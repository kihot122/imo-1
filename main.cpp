#include <algorithm>
#include <array>
#include <fstream>
#include <numeric>
#include "main.hpp"

int main(int argc, char **argv)
{
	auto Matrix = ReadMat("kroA100.tsp");
	auto Positions = ReadPos("kroA100.tsp");

	std::array<std::array<int, 3>, 100> Results;
	std::array<int, 3> BestLength{0x0fffffff, 0x0fffffff, 0x0fffffff};
	std::array<std::vector<int>, 3> BestChain;

	for (size_t i = 0; i < 100; i++)
	{
		const auto &Chain1 = Alg1(Matrix);
		const auto &Chain2 = Alg2(Matrix);
		const auto &Chain3 = Alg3(Matrix);

		int Length1 = ChainLength(Chain1, Matrix);
		int Length2 = ChainLength(Chain2, Matrix);
		int Length3 = ChainLength(Chain3, Matrix);

		Results[i] = {Length1, Length2, Length3};

		if (Length1 < BestLength[0])
		{
			BestChain[0] = Chain1;
			BestLength[0] = Length1;
		}
		if (Length2 < BestLength[1])
		{
			BestChain[1] = Chain2;
			BestLength[1] = Length2;
		}
		if (Length3 < BestLength[2])
		{
			BestChain[2] = Chain2;
			BestLength[2] = Length2;
		}
	}

	int Max1 = (*std::max_element(Results.begin(), Results.end(), [](auto &a, auto &b) { return a[0] < b[0]; }))[0];
	int Max2 = (*std::max_element(Results.begin(), Results.end(), [](auto &a, auto &b) { return a[1] < b[1]; }))[1];
	int Max3 = (*std::max_element(Results.begin(), Results.end(), [](auto &a, auto &b) { return a[2] < b[2]; }))[2];

	int Min1 = (*std::min_element(Results.begin(), Results.end(), [](auto &a, auto &b) { return a[0] < b[0]; }))[0];
	int Min2 = (*std::min_element(Results.begin(), Results.end(), [](auto &a, auto &b) { return a[1] < b[1]; }))[1];
	int Min3 = (*std::min_element(Results.begin(), Results.end(), [](auto &a, auto &b) { return a[2] < b[2]; }))[2];

	int Avg1 = std::accumulate(Results.begin(), Results.end(), 0, [](auto &acc, auto &ref) { return acc + ref[0]; }) / 100;
	int Avg2 = std::accumulate(Results.begin(), Results.end(), 0, [](auto &acc, auto &ref) { return acc + ref[1]; }) / 100;
	int Avg3 = std::accumulate(Results.begin(), Results.end(), 0, [](auto &acc, auto &ref) { return acc + ref[2]; }) / 100;

	WriteChain(BestChain[0], Positions, "Alg1.txt");
	WriteChain(BestChain[1], Positions, "Alg2.txt");
	WriteChain(BestChain[2], Positions, "Alg3.txt");

	std::fstream File("Results.txt", std::ios::out);
	File << Max1 << " " << Max2 << " " << Max3 << std::endl;
	File << Min1 << " " << Min1 << " " << Min1 << std::endl;
	File << Avg1 << " " << Avg1 << " " << Avg1 << std::endl;
}
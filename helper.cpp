#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

int ChainLength(const std::vector<int> &Cycles, const std::vector<std::vector<int>> &Matrix)
{
	int Length = 0;

	for (std::size_t i = 0; i < Cycles.size() / 2 - 1; i++)
		Length += Matrix[Cycles[i]][Cycles[i + 1]];

	Length += Matrix[Cycles[Cycles.size() / 2 - 1]][Cycles[0]];

	for (std::size_t i = Cycles.size() / 2; i < Cycles.size() - 1; i++)
		Length += Matrix[Cycles[i]][Cycles[i + 1]];

	Length += Matrix[Cycles[Cycles.size() - 1]][Cycles[Cycles.size() / 2]];

	return Length;
}

std::vector<int> RandomChain(std::vector<std::vector<int>> Matrix, int Tries)
{
	std::random_device Rand;
	std::default_random_engine Engine(Rand());
	std::vector<int> BestChain;
	int BestLength = 0x0fffffff;

	for (int i = 0; i < Tries; i++)
	{
		std::vector<int> Chain(Matrix.size());
		std::iota(Chain.begin(), Chain.end(), 0);
		std::shuffle(Chain.begin(), Chain.end(), Engine);

		if (ChainLength(Chain, Matrix) < BestLength)
		{
			BestLength = ChainLength(Chain, Matrix);
			BestChain = Chain;
		}
	}

	return BestChain;
}
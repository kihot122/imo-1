#include <vector>

int ChainLength(const std::vector<int> &Cycles, const std::vector<std::vector<int>> &Matrix)
{
	int Length = 0;

	for (size_t i = 0; i < Cycles.size() / 2 - 1; i++)
		Length += Matrix[Cycles[i]][Cycles[i + 1]];

	Length += Matrix[Cycles[Cycles.size() / 2 - 1]][Cycles[0]];

	for (size_t i = Cycles.size() / 2; i < Cycles.size() - 1; i++)
		Length += Matrix[Cycles[i]][Cycles[i + 1]];

	Length += Matrix[Cycles[Cycles.size() - 1]][Cycles[Cycles.size() / 2]];

	return Length;
}
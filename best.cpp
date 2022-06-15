#include <algorithm>
#include <iostream>
#include <random>
#include <ranges>
#include "main.hpp"

VCycle Best(VMat Matrix, int StartNode, int Timeout, int MaxBound)
{
	auto Initial = Alg2(Matrix, StartNode);
    
    std::random_device Rand;
	std::default_random_engine Engine(Rand());
	std::uniform_int_distribution Dist(2, MaxBound);
	auto View = std::views::iota(0, (int)Matrix.size());
	auto Range = std::vector<int>(View.begin(), View.end());

	auto Current = EdgeSteep(Matrix, Initial);
	auto CurrentLength = ChainLength(Matrix, Current);

	for (int i = 0; i < Timeout; i++)
	{
		auto Next = Current;
		std::shuffle(Range.begin(), Range.end(), Engine);
		int Bound = Dist(Engine);

		auto Temp = Next[Range[0]];
		for (int j : std::views::iota(0, Bound))
		{
			int l = (Bound - j + 1) % (Bound + 1);
			int r = Bound - j;
			Next[Range[l]] = Next[Range[r]];
		}
		Next[Range[1]] = Temp;

		auto Result = EdgeSteep(Matrix, Next);
		auto ResultLength = ChainLength(Matrix, Result);
		if (ResultLength < CurrentLength)
		{
			Current = Result;
			CurrentLength = ResultLength;
			i = 0;
		}
	}

	return Current;
}
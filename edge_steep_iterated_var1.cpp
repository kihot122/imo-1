#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <ranges>
#include "main.hpp"

VCycle EdgeSteepIteratedVar1(VMat Matrix, VCycle Cycles, int Tries)
{
	std::random_device Rand;
	std::default_random_engine Engine(Rand());
	std::uniform_int_distribution Dist(0, 1);
	auto View = std::views::iota(0, (int)Matrix.size());
	auto Range = std::vector<int>(View.begin(), View.end());

	auto Current = EdgeSteep(Matrix, Cycles);
	auto CurrentLength = ChainLength(Current, Matrix);
	for (int i : std::views::iota(0, Tries))
	{
		auto Next = Current;
		std::shuffle(Range.begin(), Range.end(), Engine);
		// Exchange 3 vertices
		// 1 -> 2, 2 -> 3, 3 -> 1
		if (not Dist(Engine))
		{
			auto Temp = Next[Range[0]];
			Next[Range[0]] = Next[Range[2]];
			Next[Range[2]] = Next[Range[1]];
			Next[Range[1]] = Temp;
		}
		// But sometimes also 5 vertices
		// 1 -> 2, 2 -> 3, 3 -> 4, 4 -> 5, 5 -> 1
		else
		{
			auto Temp = Next[Range[0]];
			Next[Range[0]] = Next[Range[4]];
			Next[Range[4]] = Next[Range[3]];
			Next[Range[3]] = Next[Range[2]];
			Next[Range[2]] = Next[Range[1]];
			Next[Range[1]] = Temp;
		}

		auto Result = EdgeSteep(Matrix, Next);
		auto ResultLength = ChainLength(Result, Matrix);
		if (ResultLength < CurrentLength)
		{
			Current = Result;
			CurrentLength = ResultLength;
		}
	}

	return Current;
}
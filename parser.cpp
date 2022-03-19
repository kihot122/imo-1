#include <cmath>
#include <fstream>
#include "main.hpp"

std::vector<std::vector<int>> ReadMat(const std::string &Filename)
{
	auto &&Positions = ReadPos(Filename);
	std::vector<std::vector<int>> Matrix;
	for (auto &X1 : Positions)
	{
		std::vector<int> Temp;
		for (auto &X2 : Positions)
		{
			Temp.push_back(static_cast<int>(std::sqrt(std::pow(std::get<0>(X1) - std::get<0>(X2), 2) + std::pow(std::get<1>(X1) - std::get<1>(X2), 2))));
		}
		Matrix.push_back(Temp);
	}

	return Matrix;
}

std::vector<std::tuple<int, int>> ReadPos(const std::string &Filename)
{
	std::fstream Stream(Filename, std::ios::in);
	std::string Token;

	while (Stream >> Token)
	{
		if (Token == "NODE_COORD_SECTION")
			break;
	}

	std::vector<std::tuple<int, int>> Positions;

	while (Stream >> Token)
	{
		if (Token == "EOF")
			break;

		std::string X, Y;
		Stream >> X >> Y;
		Positions.push_back({std::stoi(X), std::stoi(Y)});
	}

	return Positions;
}

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

void WritePos(const std::vector<std::tuple<int, int>> &Vec, const std::string &Filename)
{
	std::fstream Stream(Filename, std::ios::out);
	for (auto &[X, Y] : Vec)
		Stream << X << " " << Y << std::endl;

	// Stream << std::format("{} {}\n", X, Y) std::format in GCC when?
}

void WriteChain(const std::vector<int> &Cycles, const std::vector<std::tuple<int, int>> &Positions, const std::string &Filename)
{
	std::vector<std::tuple<int, int>> Vec;

	for (int Index : Cycles)
		Vec.push_back(Positions[Index]);

	WritePos(Vec, Filename);
}
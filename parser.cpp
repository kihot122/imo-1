#include <cmath>
#include <fstream>
#include "parser.hpp"

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

void WritePos(const std::vector<std::tuple<int, int>> &Vec, const std::string &Filename)
{
	std::fstream Stream(Filename, std::ios::out);
	for (auto &[X, Y] : Vec)
		Stream << X << "\t" << Y << std::endl;

	// Stream << std::format("{}\t{}\n", X, Y) std::format in GCC when?
}
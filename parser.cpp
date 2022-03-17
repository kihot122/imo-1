#include <fstream>
#include "parser.hpp"

std::vector<std::vector<int>> ReadMat(std::string Filename)
{
	auto &&Positions = ReadPos(Filename);

	return std::vector<std::vector<int>>();
}

std::vector<std::tuple<int, int>> ReadPos(std::string Filename)
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
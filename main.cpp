#include "main.hpp"

int main(int argc, char **argv)
{
	auto Matrix = ReadMat("kroA100.tsp");
	auto Positions = ReadPos("kroA100.tsp");

	WriteChain(Alg2(Matrix), Positions, "output");
}
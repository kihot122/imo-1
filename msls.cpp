#include <chrono>
#include <iostream>

#include "main.hpp"

std::vector<int> msls(std::vector<std::vector<int>> matrix, double timeS, int iterations)
{
	std::vector<int> x;
	std::vector<int> y;

	x = EdgeSteep(matrix, Alg2(matrix));

	auto TimeBefore = std::chrono::high_resolution_clock::now();
	int c = 0;
	while (std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - TimeBefore).count() <= timeS)
	{
		if (c >= iterations)
		{
			break;
		}

		y = EdgeSteep(matrix, Alg2(matrix));

		if (ChainLength(x, matrix) > ChainLength(y, matrix))
		{
			// std::cout << "UPDATE" << "\n";
			x = y;
		}
		c++;
	}

	return x;
}
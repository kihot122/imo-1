#include <iostream>
#include <vector>
#include "main.hpp"

std::vector<int> Alg1(std::vector<std::vector<int>> &matrix, int cycle1Start, int cycle2Start)
{
	int pointsCount = matrix.size();

	std::vector<bool> left(pointsCount, true);
	int assigned = 0;

	std::vector<int> cycle[2];

	if (cycle1Start == cycle2Start)
	{
		throw "Cycles can't start at the same point";
	}

	left[cycle1Start] = false;
	left[cycle2Start] = false;
	assigned = 2;

	cycle[0].push_back(cycle1Start);
	cycle[1].push_back(cycle2Start);

	int cycleIndex;
	int closestId;
	while (assigned != pointsCount)
	{
		cycleIndex = assigned % 2;
		closestId = pointsCount;

		for (int j = 0; j < pointsCount; j++)
		{
			std::cout << left[j] << '\n';
			if (left[j])
			{
				if (closestId == pointsCount)
				{
					closestId = j;
					continue;
				}

				std::cout << matrix[cycle[cycleIndex].back()][j] << '\n' << matrix[cycle[cycleIndex].back()][closestId] << "\n\n";
				if (matrix[cycle[cycleIndex].back()][j] <= matrix[cycle[cycleIndex].back()][closestId])
				{
					closestId = j;
				}
			}
		}

		left[closestId] = false;
		cycle[cycleIndex].push_back(closestId);
		assigned++;
	}

	cycle[0].insert(cycle[0].end(), cycle[1].begin(), cycle[1].end());
	return cycle[0];
}
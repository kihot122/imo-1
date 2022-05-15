#include <iostream>
#include <vector>
#include "main.hpp"
#include <stdlib.h>
#include <time.h>

std::vector<int> Alg1(std::vector<std::vector<int>> matrix, int startnode)
{
	int pointsCount = matrix.size();

	std::vector<bool> left(pointsCount, true);
	int assigned = 0;

	std::vector<int> cycle[2];

	srand(time(NULL) + rand());
	int cycle1Start = startnode == -1 ? rand() % pointsCount : startnode;
	left[cycle1Start] = false;
	int cycle2Start = pointsCount;

	for (int i = 0; i < pointsCount; i++)
	{
		if (left[i])
		{
			if (cycle2Start == pointsCount)
			{
				cycle2Start = i;
				continue;
			}

			if (matrix[cycle1Start][i] >= matrix[cycle1Start][cycle2Start])
			{
				cycle2Start = i;
			}
		}
	}

	left[cycle2Start] = false;
	assigned = 2;

	cycle[0].push_back(cycle1Start);
	cycle[1].push_back(cycle2Start);

	int closestId;
	int cycleIndex;
	while (assigned != pointsCount)
	{
		cycleIndex = assigned % 2;
		closestId = pointsCount;

		for (int j = 0; j < pointsCount; j++)
		{
			if (left[j])
			{
				if (closestId == pointsCount)
				{
					closestId = j;
					continue;
				}

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
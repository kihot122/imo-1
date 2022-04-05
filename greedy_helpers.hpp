#pragma once

#include <string>
#include <tuple>
#include <vector>



std::array<int, 3> getFromCyclePrevMiddleNext(std::vector<int> &Cycle, int middleIndex);

int crossDeltaProfit(std::vector<std::vector<int>> &Matrix, std::vector<int> &CycleA, std::vector<int> &CycleB, int swapIndexA, int swapIndexB);

int validateCycle(std::vector<std::vector<int>> &matrix, std::vector<int> &cycle);

bool CrossGreedy(std::vector<std::vector<int>> &Matrix, std::vector<int> &CycleA, std::vector<int> &CycleB, int changesCount);

int localPointsSwapProfit(std::vector<std::vector<int>> &matrix, std::vector<int> &cycle, int swapIndexA, int swapIndexB);

int localSectionsSwapProfit(std::vector<std::vector<int>> &matrix, std::vector<int> &cycle, int sectionIndexA, int sectionIndexB);

void swapLocalPoints(std::vector<int> &cycle, int indexA, int indexB);

void swapLocalSections(std::vector<int> &cycle, int sectionIndexA, int sectionIndexB);

bool localCycleOptimisation(std::vector<std::vector<int>> &Matrix, std::vector<int> &cycle, int changesCount, bool sections);

bool CrossGreedyRandom(std::vector<std::vector<int>> &Matrix, std::vector<int> &CycleA, std::vector<int> &CycleB, int changesCount);

bool localCycleRandom(std::vector<std::vector<int>> &Matrix, std::vector<int> &cycle, int changesCount, bool sections);


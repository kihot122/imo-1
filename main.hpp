#pragma once

#include <string>
#include <tuple>
#include <vector>

std::vector<std::vector<int>> ReadMat(const std::string &Filename);
std::vector<std::tuple<int, int>> ReadPos(const std::string &Filename);
void WritePos(const std::vector<std::tuple<int, int>> &Vec, const std::string &Filename);
void WriteChain(const std::vector<int> &Cycles, const std::vector<std::tuple<int, int>> &Positions, const std::string &Filename);
int ChainLength(const std::vector<int> &Cycles, const std::vector<std::vector<int>> &Matrix);

/**
 * @brief Get the random chain, best of number of rolls
 *
 * @param Matrix Adjacency matrix
 * @param Tries Number of rolls
 * @return 2 consecutive cycles
 */
std::vector<int> RandomChain(std::vector<std::vector<int>> Matrix, int Tries = 100);

std::vector<int> Alg1(std::vector<std::vector<int>> Matrix, int StartNode = -1);
std::vector<int> Alg2(std::vector<std::vector<int>> Matrix, int StartNode = -1);
std::vector<int> Alg3(std::vector<std::vector<int>> Matrix, int StartNode = -1);

/**
 * @brief Vertex exchange algorithm between 2 cycles, greedy version.
 *
 * @param Matrix Adjacency matrix
 * @param Cycles 2 consecutive cycles
 * @return 2 improved consecutive cycles
 */
std::vector<int> CrossGreedy(std::vector<std::vector<int>> Matrix, std::vector<int> Cycles, int changesCount);

/**
 * @brief Vertex exchange algorithm between 2 cycles, steep version.
 *
 * @param Matrix Adjacency matrix
 * @param Cycles 2 consecutive cycles
 * @return 2 improved consecutive cycles
 */
std::vector<int> CrossSteep(std::vector<std::vector<int>> Matrix, std::vector<int> Cycles);

/**
 * @brief Vertex and edge exchange algorithm inside 2 cycles, greedy version.
 *
 * @param Matrix Adjacency matrix
 * @param Cycles 2 consecutive cycles
 * @return 2 improved consecutive cycles
 */
std::vector<int> LocalGreedy(std::vector<std::vector<int>> Matrix, std::vector<int> Cycles, int changesCount);

/**
 * @brief Vertex and edge exchange algorithm inside 2 cycles, steep version.
 *
 * @param Matrix Adjacency matrix
 * @param Cycles 2 consecutive cycles
 * @return 2 improved consecutive cycles
 */
std::vector<int> LocalSteep(std::vector<std::vector<int>> Matrix, std::vector<int> Cycles);

/**
 * @brief Return adjacent indeses from cycle.
 * 
 * @param Cycle the cycle
 * @param middleIndex cycle index of returned and in between of returned values
 * 
 * @return array of previous, pointed and next value of specified by given index
 **/
std::array<int, 3> getFromCyclePrevMiddleNext(std::vector<int> &Cycle, int middleIndex); 


// std::vector<int> crossCyclesGreedyOptimisation(std::vector<std::vector<int>> Matrix, std::vector<int> Cycles, int changesCount);


// std::vector<int> localCyclesGreedyOptimisation(std::vector<std::vector<int>> Matrix, std::vector<int> cycles, int changesCount);

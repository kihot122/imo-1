#pragma once

#include <string>
#include <tuple>
#include <vector>

std::vector<std::vector<int>> ReadMat(const std::string &Filename);
std::vector<std::tuple<int, int>> ReadPos(const std::string &Filename);
void WritePos(const std::vector<std::tuple<int, int>> &Vec, const std::string &Filename);
void WriteChain(const std::vector<int> &Cycles, const std::vector<std::tuple<int, int>> &Positions, const std::string &Filename);
int ChainLength(const std::vector<int> &Cycles, const std::vector<std::vector<int>> &Matrix);

std::vector<int> Alg1(std::vector<std::vector<int>> Matrix, int StartNode = -1);
std::vector<int> Alg2(std::vector<std::vector<int>> Matrix, int StartNode = -1);
std::vector<int> Alg3(std::vector<std::vector<int>> Matrix, int StartNode = -1);

/**
 * @brief Vertex exchange algorithm between 2 cycles, steep version.
 *
 * @param Matrix Adjacency matrix
 * @param Cycles 2 consecutive cycles
 * @return std::vector<int> 2 improved consecutive cycles
 */
std::vector<int> CrossSteep(std::vector<std::vector<int>> Matrix, std::vector<int> Cycles);

/**
 * @brief Vertex and edge exchange algorithm inside 2 cycles, steep version.
 *
 * @param Matrix Adjacency matrix
 * @param Cycles 2 consecutive cycles
 * @return std::vector<int> 2 improved consecutive cycles
 */
std::vector<int> LocalSteep(std::vector<std::vector<int>> Matrix, std::vector<int> Cycles);
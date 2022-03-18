#pragma once

#include <string>
#include <tuple>
#include <vector>

std::vector<std::vector<int>> ReadMat(const std::string &Filename);
std::vector<std::tuple<int, int>> ReadPos(const std::string &Filename);
void WritePos(const std::vector<std::tuple<int, int>> &Vec, const std::string &Filename);

std::vector<int> Alg1(const std::vector<std::vector<int>> &Matrix);
std::vector<int> Alg2(const std::vector<std::vector<int>> &Matrix);
std::vector<int> Alg3(const std::vector<std::vector<int>> &Matrix);
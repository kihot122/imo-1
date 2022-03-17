#include <string>
#include <tuple>
#include <vector>

std::vector<std::vector<int>> ReadMat(std::string Filename);
std::vector<std::tuple<int, int>> ReadPos(std::string Filename);
void Write(const std::vector<std::tuple<int, int>> &V, std::string Filename);
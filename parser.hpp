#include <fstream>
#include <tuple>
#include <vector>

std::vector<std::vector<int>> ReadMat(std::string &Filename);
std::vector<std::tuple<int, int>> ReadPos(std::string &Filename);
void Write(std::vector<std::tuple<int, int>> &V, std::string &Filename);
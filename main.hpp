#pragma once

#include <string>
#include <tuple>
#include <vector>

typedef std::vector<std::vector<int>> VMat;
typedef std::vector<int> VCycle;

VMat ReadMat(const std::string &Filename);
std::vector<std::tuple<int, int>> ReadPos(const std::string &Filename);
void WritePos(const std::vector<std::tuple<int, int>> &Vec, const std::string &Filename);
void WriteChain(const VCycle &Cycles, const std::vector<std::tuple<int, int>> &Positions, const std::string &Filename);
int ChainLength(const VMat &Matrix, const VCycle &Cycles);

/**
 * @brief Get the random chain, best of number of rolls
 *
 * @param Matrix Adjacency matrix
 * @param Tries Number of rolls
 * @return 2 consecutive cycles
 */
VCycle RandomChain(VMat Matrix, int Tries = 100);

VCycle Alg1(VMat Matrix, int StartNode = -1);
VCycle Alg2(VMat Matrix, int StartNode = -1);
VCycle Alg3(VMat Matrix, int StartNode = -1);

VCycle VertexRandom(VMat Matrix, VCycle Cycles, uint64_t TimeNs);
VCycle VertexGreedy(VMat Matrix, VCycle Cycles);
VCycle VertexSteep(VMat Matrix, VCycle Cycles);
VCycle EdgeRandom(VMat Matrix, VCycle Cycles, uint64_t TimeNs);
VCycle EdgeGreedy(VMat Matrix, VCycle Cycles);
VCycle EdgeSteep(VMat Matrix, VCycle Cycles);

/**
 * @brief Improved algorithm upon edge exchange. Modification - best moves list.
 *
 * @param Matrix Adjacency matrix
 * @param Cycles Base solution
 * @return VCycle Improved base solution
 */
VCycle EdgeSteepVar1(VMat Matrix, VCycle Cycles);

/**
 * @brief Improved algorithm upon edge exchange. Modification - candidate moves.
 *
 * @param Matrix Adjacency matrix
 * @param Cycles Base solution
 * @return VCycle Improved base solution
 */
VCycle EdgeSteepVar2(VMat Matrix, VCycle Cycles, int CandidatesCount = 5);

VCycle EdgeSteepIteratedVar1(VMat Matrix, VCycle Cycles, int Tries = 100);

/**
 * @brief Return adjacent indeses from cycle.
 *
 * @param Cycle the cycle
 * @param MiddleIndex cycle index of returned and in between of returned values
 *
 * @return array of previous, pointed and next value of specified by given index
 **/
std::array<int, 3> getFromCyclePrevMiddleNext(VCycle &Cycle, int MiddleIndex);

/** ILS with destroy repair perturbation
 * ILS (Iterated Local Search) for TSP problem.
 * Operates with perturbation destroy-repair type with 20% ratio to destroy
 * .
 **/
class ILSDestroyRepair
{
  private:
	enum class ExchangeType : bool;
	void edgeSteep(std::vector<std::vector<int>> &Matrix, std::vector<int> &Cycles);
	void nearestGreedy(std::vector<std::vector<int>> &matrix, std::vector<int> &cycles, std::vector<bool> &left, int notAssigned);
	void destroyRepairPerturbation(std::vector<std::vector<int>> &matrix, std::vector<int> &cycles, float destroyRatio = 0.2);
	int cyclesLength(std::vector<std::vector<int>> &matrix, std::vector<int> &cycles);

  public:
	/** Updates given cycles.
	 * @brief Local search for TSP, steep, edges.
	 *
	 * @param matrix Adjacency matrix
	 * @param cycles Base solution: two cycles same size concatenated in one vector (if vector not odd, first cycle is bigger).
	 *               Every element occures only once, so each cycle elemnts loop in connection.
	 * @param time Max iterations time
	 * @param iterations Max iteration count
	 *
	 * */
	std::vector<int> ils(std::vector<std::vector<int>> matrix, std::vector<int> cycles, double time = 3600, int iterations = 100);
};

std::vector<int> msls(std::vector<std::vector<int>> matrix, double timeS = 3600, int iterations = 100);

/**
 * @brief Genetic algorithm with egde-intersection operator
 *
 * @param Matrix Adkacency matrix
 * @param CrossPopulationSize Best n from population that undergo childbirth
 * @return Best of all generations
 */
VCycle Genetic(VMat Matrix, int CrossPopulationSize = 20, int Epoch = 10);
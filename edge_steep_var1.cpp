#include <algorithm>
#include <set>
#include <vector>
#include "main.hpp"

enum class ExchangeType : bool
{
	LOCAL,
	CROSS,
};

enum class Validity : uint8_t
{
	APPLICABLE,
	DELAYED,
	REJECTED,
};

struct Exchange
{
	int A, B;
	int PreA, PreB;
	int PostA, PostB;
	int X, Y;
	int PreX, PreY;
	int PostX, PostY;
	int CycleIndex;
	int Delta;
	ExchangeType Type;

	Validity Validate(std::array<std::vector<int>, 2> &CyclesArr) const
	{
		switch (Type)
		{
			case ExchangeType::LOCAL:
			{
				auto &Cycle = CyclesArr[CycleIndex];

				if (Cycle[A] == X and Cycle[B] == Y and Cycle[PostA] == PostX and Cycle[PostB] == PostY)
					return Validity::APPLICABLE;

				if (Cycle[A] == PostX and Cycle[B] == PostY and Cycle[PostA] == X and Cycle[PostB] == Y)
					return Validity::APPLICABLE;

				if (Cycle[A] == PostX and Cycle[B] == Y and Cycle[PostA] == X and Cycle[PostB] == PostY)
					return Validity::DELAYED;

				if (Cycle[A] == X and Cycle[B] == PostY and Cycle[PostA] == PostX and Cycle[PostB] == Y)
					return Validity::DELAYED;

				return Validity::REJECTED;
			}
			case ExchangeType::CROSS:
			{
				auto &CycleA = CyclesArr[0];
				auto &CycleB = CyclesArr[1];

				if (CycleA[A] == X and CycleB[B] == Y and CycleA[PostA] == PostX and CycleB[PostB] == PostY and CycleA[PreA] == PreX and CycleB[PreB] == PreY)
					return Validity::APPLICABLE;

				return Validity::REJECTED;
			}
		}
	}

	void Apply(std::array<std::vector<int>, 2> &CyclesArr) const
	{
		switch (Type)
		{
			case ExchangeType::LOCAL:
			{
				std::reverse(CyclesArr[CycleIndex].begin() + A + 1, CyclesArr[CycleIndex].begin() + B + 1);
				break;
			}
			case ExchangeType::CROSS:
			{
				std::swap(CyclesArr[0][A], CyclesArr[1][B]);
				break;
			}
		}
	}
};

struct ExchangeCmp
{
	bool operator()(const Exchange &A, const Exchange &B) const
	{
		return A.Delta <= B.Delta;
	}
};

std::multiset<Exchange, ExchangeCmp> SearchNew(const std::vector<std::vector<int>> &Matrix, const std::array<std::vector<int>, 2> &CyclesArr, const Exchange &Move)
{
	std::multiset<Exchange, ExchangeCmp> BestMoves;

	if (Move.Type == ExchangeType::LOCAL)
	{
		// LOCAL-LOCAL
		{
			auto &Cycle = CyclesArr[Move.CycleIndex];
			for (int A = 0; A < Cycle.size(); A++)
			{
				for (int B = A + 1; B < Cycle.size(); B++)
				{
					int Size = static_cast<int>(Cycle.size());

					int AMin = A - 1 < 0 ? Size - 1 : A - 1;
					int BMin = B - 1 < 0 ? Size - 1 : B - 1;

					int AMax = A + 1 == Size ? 0 : A + 1;
					int BMax = B + 1 == Size ? 0 : B + 1;

					//(A to B) + (A+1 to B+1) - (A to A+1) - (B + B+1)
					int Delta = Matrix[Cycle[A]][Cycle[B]] + Matrix[Cycle[AMax]][Cycle[BMax]] - Matrix[Cycle[A]][Cycle[AMax]] - Matrix[Cycle[B]][Cycle[BMax]];

					if (Delta < 0)
					{
						BestMoves.insert(Exchange{
							.A = A,
							.B = B,
							.PreA = AMin,
							.PreB = BMin,
							.PostA = AMax,
							.PostB = BMax,
							.X = Cycle[A],
							.Y = Cycle[B],
							.PreX = Cycle[AMin],
							.PreY = Cycle[BMin],
							.PostX = Cycle[AMax],
							.PostY = Cycle[BMax],
							.CycleIndex = Move.CycleIndex,
							.Delta = Delta,
							.Type = ExchangeType::LOCAL,
						});
					}
				}
			}
		}

		// LOCAL-CROSS
		{
			auto &CycleA = CyclesArr[0];
			auto &CycleB = CyclesArr[1];

			for (int A = Move.CycleIndex == 0 ? Move.A : 0; A < (Move.CycleIndex == 0 ? Move.B : CycleA.size()); A++)
			{
				for (int B = Move.CycleIndex == 1 ? Move.A : 0; B < (Move.CycleIndex == 1 ? Move.B : CycleB.size()); B++)
				{
					int SizeA = static_cast<int>(CycleA.size());
					int SizeB = static_cast<int>(CycleB.size());

					int AMin = A - 1 < 0 ? SizeA - 1 : A - 1;
					int BMin = B - 1 < 0 ? SizeB - 1 : B - 1;

					int AMax = A + 1 == SizeA ? 0 : A + 1;
					int BMax = B + 1 == SizeB ? 0 : B + 1;

					//(A-1 to B) + (A+1 to B) + (B-1 to A) + (B+1 to A)
					//- (A-1 to A) - (A+1 to A) - (B-1 to B) - (B+1 to B)
					int Delta = Matrix[CycleA[AMin]][CycleB[B]] + Matrix[CycleA[AMax]][CycleB[B]] + Matrix[CycleB[BMin]][CycleA[A]] + Matrix[CycleB[BMax]][CycleA[A]] - Matrix[CycleA[AMin]][CycleA[A]] - Matrix[CycleA[AMax]][CycleA[A]] -
								Matrix[CycleB[BMin]][CycleB[B]] - Matrix[CycleB[BMax]][CycleB[B]];

					if (Delta < 0)
					{
						BestMoves.insert(Exchange{
							.A = A,
							.B = B,
							.PreA = AMin,
							.PreB = BMin,
							.PostA = AMax,
							.PostB = BMax,
							.X = CycleA[A],
							.Y = CycleB[B],
							.PreX = CycleA[AMin],
							.PreY = CycleB[BMin],
							.PostX = CycleA[AMax],
							.PostY = CycleB[BMax],
							.Delta = Delta,
							.Type = ExchangeType::CROSS,
						});
					}
				}
			}
		}
	}
	if (Move.Type == ExchangeType::CROSS)
	{
		// CROSS-LOCAL
		{
			auto &Cycle = CyclesArr[0];
			for (int A : {Move.PreA, Move.A})
			{
				for (int B = 0; B < Cycle.size(); B++)
				{
					if (A == B)
						continue;

					int Size = static_cast<int>(Cycle.size());

					int AMin = A - 1 < 0 ? Size - 1 : A - 1;
					int BMin = B - 1 < 0 ? Size - 1 : B - 1;

					int AMax = A + 1 == Size ? 0 : A + 1;
					int BMax = B + 1 == Size ? 0 : B + 1;

					//(A to B) + (A+1 to B+1) - (A to A+1) - (B + B+1)
					int Delta = Matrix[Cycle[A]][Cycle[B]] + Matrix[Cycle[AMax]][Cycle[BMax]] - Matrix[Cycle[A]][Cycle[AMax]] - Matrix[Cycle[B]][Cycle[BMax]];

					if (Delta < 0)
					{
						BestMoves.insert(Exchange{
							.A = A,
							.B = B,
							.PreA = AMin,
							.PreB = BMin,
							.PostA = AMax,
							.PostB = BMax,
							.X = Cycle[A],
							.Y = Cycle[B],
							.PreX = Cycle[AMin],
							.PreY = Cycle[BMin],
							.PostX = Cycle[AMax],
							.PostY = Cycle[BMax],
							.CycleIndex = 0,
							.Delta = Delta,
							.Type = ExchangeType::LOCAL,
						});
					}
				}
			}
		}
		{
			auto &Cycle = CyclesArr[1];
			for (int A : {Move.PreB, Move.B})
			{
				for (int B = 0; B < Cycle.size(); B++)
				{
					if (A == B)
						continue;

					int Size = static_cast<int>(Cycle.size());

					int AMin = A - 1 < 0 ? Size - 1 : A - 1;
					int BMin = B - 1 < 0 ? Size - 1 : B - 1;

					int AMax = A + 1 == Size ? 0 : A + 1;
					int BMax = B + 1 == Size ? 0 : B + 1;

					//(A to B) + (A+1 to B+1) - (A to A+1) - (B + B+1)
					int Delta = Matrix[Cycle[A]][Cycle[B]] + Matrix[Cycle[AMax]][Cycle[BMax]] - Matrix[Cycle[A]][Cycle[AMax]] - Matrix[Cycle[B]][Cycle[BMax]];

					if (Delta < 0)
					{
						BestMoves.insert(Exchange{
							.A = A,
							.B = B,
							.PreA = AMin,
							.PreB = BMin,
							.PostA = AMax,
							.PostB = BMax,
							.X = Cycle[A],
							.Y = Cycle[B],
							.PreX = Cycle[AMin],
							.PreY = Cycle[BMin],
							.PostX = Cycle[AMax],
							.PostY = Cycle[BMax],
							.CycleIndex = 1,
							.Delta = Delta,
							.Type = ExchangeType::LOCAL,
						});
					}
				}
			}
		}

		// CROSS-CROSS
		{
			auto &CycleA = CyclesArr[0];
			auto &CycleB = CyclesArr[1];
			auto &A = Move.A;

			for (int B = 0; B < CycleB.size(); B++)
			{
				int SizeA = static_cast<int>(CycleA.size());
				int SizeB = static_cast<int>(CycleB.size());

				int AMin = A - 1 < 0 ? SizeA - 1 : A - 1;
				int BMin = B - 1 < 0 ? SizeB - 1 : B - 1;

				int AMax = A + 1 == SizeA ? 0 : A + 1;
				int BMax = B + 1 == SizeB ? 0 : B + 1;

				//(A-1 to B) + (A+1 to B) + (B-1 to A) + (B+1 to A)
				//- (A-1 to A) - (A+1 to A) - (B-1 to B) - (B+1 to B)
				int Delta = Matrix[CycleA[AMin]][CycleB[B]] + Matrix[CycleA[AMax]][CycleB[B]] + Matrix[CycleB[BMin]][CycleA[A]] + Matrix[CycleB[BMax]][CycleA[A]] - Matrix[CycleA[AMin]][CycleA[A]] - Matrix[CycleA[AMax]][CycleA[A]] -
							Matrix[CycleB[BMin]][CycleB[B]] - Matrix[CycleB[BMax]][CycleB[B]];

				if (Delta < 0)
				{
					BestMoves.insert(Exchange{
						.A = A,
						.B = B,
						.PreA = AMin,
						.PreB = BMin,
						.PostA = AMax,
						.PostB = BMax,
						.X = CycleA[A],
						.Y = CycleB[B],
						.PreX = CycleA[AMin],
						.PreY = CycleB[BMin],
						.PostX = CycleA[AMax],
						.PostY = CycleB[BMax],
						.Delta = Delta,
						.Type = ExchangeType::CROSS,
					});
				}
			}
		}
		{
			auto &CycleA = CyclesArr[0];
			auto &CycleB = CyclesArr[1];
			auto &B = Move.B;

			for (int A = 0; A < CycleA.size(); A++)
			{
				int SizeA = static_cast<int>(CycleA.size());
				int SizeB = static_cast<int>(CycleB.size());

				int AMin = A - 1 < 0 ? SizeA - 1 : A - 1;
				int BMin = B - 1 < 0 ? SizeB - 1 : B - 1;

				int AMax = A + 1 == SizeA ? 0 : A + 1;
				int BMax = B + 1 == SizeB ? 0 : B + 1;

				//(A-1 to B) + (A+1 to B) + (B-1 to A) + (B+1 to A)
				//- (A-1 to A) - (A+1 to A) - (B-1 to B) - (B+1 to B)
				int Delta = Matrix[CycleA[AMin]][CycleB[B]] + Matrix[CycleA[AMax]][CycleB[B]] + Matrix[CycleB[BMin]][CycleA[A]] + Matrix[CycleB[BMax]][CycleA[A]] - Matrix[CycleA[AMin]][CycleA[A]] - Matrix[CycleA[AMax]][CycleA[A]] -
							Matrix[CycleB[BMin]][CycleB[B]] - Matrix[CycleB[BMax]][CycleB[B]];

				if (Delta < 0)
				{
					BestMoves.insert(Exchange{
						.A = A,
						.B = B,
						.PreA = AMin,
						.PreB = BMin,
						.PostA = AMax,
						.PostB = BMax,
						.X = CycleA[A],
						.Y = CycleB[B],
						.PreX = CycleA[AMin],
						.PreY = CycleB[BMin],
						.PostX = CycleA[AMax],
						.PostY = CycleB[BMax],
						.Delta = Delta,
						.Type = ExchangeType::CROSS,
					});
				}
			}
		}
	}

	return BestMoves;
}

std::multiset<Exchange, ExchangeCmp> Initial(const std::vector<std::vector<int>> &Matrix, const std::array<std::vector<int>, 2> &CyclesArr)
{
	std::multiset<Exchange, ExchangeCmp> BestMoves;

	for (int i = 0; auto &&Cycle : CyclesArr)
	{
		for (int A = 0; A < Cycle.size(); A++)
		{
			for (int B = A + 1; B < Cycle.size(); B++)
			{
				int Size = static_cast<int>(Cycle.size());

				int AMin = A - 1 < 0 ? Size - 1 : A - 1;
				int BMin = B - 1 < 0 ? Size - 1 : B - 1;

				int AMax = A + 1 == Size ? 0 : A + 1;
				int BMax = B + 1 == Size ? 0 : B + 1;

				//(A to B) + (A+1 to B+1) - (A to A+1) - (B + B+1)
				int Delta = Matrix[Cycle[A]][Cycle[B]] + Matrix[Cycle[AMax]][Cycle[BMax]] - Matrix[Cycle[A]][Cycle[AMax]] - Matrix[Cycle[B]][Cycle[BMax]];

				if (Delta < 0)
				{
					BestMoves.insert(Exchange{
						.A = A,
						.B = B,
						.PreA = AMin,
						.PreB = BMin,
						.PostA = AMax,
						.PostB = BMax,
						.X = Cycle[A],
						.Y = Cycle[B],
						.PreX = Cycle[AMin],
						.PreY = Cycle[BMin],
						.PostX = Cycle[AMax],
						.PostY = Cycle[BMax],
						.CycleIndex = i,
						.Delta = Delta,
						.Type = ExchangeType::LOCAL,
					});
				}
			}
		}
		i++;
	}

	auto &CycleA = CyclesArr[0];
	auto &CycleB = CyclesArr[1];
	for (int A = 0; A < CycleA.size(); A++)
	{
		for (int B = 0; B < CycleB.size(); B++)
		{
			int SizeA = static_cast<int>(CycleA.size());
			int SizeB = static_cast<int>(CycleB.size());

			int AMin = A - 1 < 0 ? SizeA - 1 : A - 1;
			int BMin = B - 1 < 0 ? SizeB - 1 : B - 1;

			int AMax = A + 1 == SizeA ? 0 : A + 1;
			int BMax = B + 1 == SizeB ? 0 : B + 1;

			//(A-1 to B) + (A+1 to B) + (B-1 to A) + (B+1 to A)
			//- (A-1 to A) - (A+1 to A) - (B-1 to B) - (B+1 to B)
			int Delta = Matrix[CycleA[AMin]][CycleB[B]] + Matrix[CycleA[AMax]][CycleB[B]] + Matrix[CycleB[BMin]][CycleA[A]] + Matrix[CycleB[BMax]][CycleA[A]] - Matrix[CycleA[AMin]][CycleA[A]] - Matrix[CycleA[AMax]][CycleA[A]] -
						Matrix[CycleB[BMin]][CycleB[B]] - Matrix[CycleB[BMax]][CycleB[B]];

			if (Delta < 0)
			{
				BestMoves.insert(Exchange{
					.A = A,
					.B = B,
					.PreA = AMin,
					.PreB = BMin,
					.PostA = AMax,
					.PostB = BMax,
					.X = CycleA[A],
					.Y = CycleB[B],
					.PreX = CycleA[AMin],
					.PreY = CycleB[BMin],
					.PostX = CycleA[AMax],
					.PostY = CycleB[BMax],
					.Delta = Delta,
					.Type = ExchangeType::CROSS,
				});
			}
		}
	}

	return BestMoves;
}

std::vector<int> EdgeSteepVar1(std::vector<std::vector<int>> Matrix, std::vector<int> Cycles)
{
	std::array CyclesArr = {std::vector<int>(Cycles.begin(), Cycles.begin() + Cycles.size() / 2), std::vector<int>(Cycles.begin() + Cycles.size() / 2, Cycles.end())};
	auto BestMoves = Initial(Matrix, CyclesArr);
	bool Found;

	do
	{
		Found = false;
		std::multiset<Exchange, ExchangeCmp> NewMoves;

		for (auto Iter = BestMoves.begin(); Iter != BestMoves.end();)
		{
			if (Iter->Validate(CyclesArr) == Validity::APPLICABLE)
			{
				Iter->Apply(CyclesArr);
				NewMoves.merge(SearchNew(Matrix, CyclesArr, *Iter));
				Iter = BestMoves.erase(Iter);
				Found = true;
				break;
			}
			else if (Iter->Validate(CyclesArr) == Validity::DELAYED)
			{
				Iter++;
			}
			else if (Iter->Validate(CyclesArr) == Validity::REJECTED)
			{
				Iter = BestMoves.erase(Iter);
			}
		}

		BestMoves.merge(NewMoves);
	} while (Found);

	std::vector<int> Temp = CyclesArr[0];
	Temp.insert(Temp.end(), CyclesArr[1].begin(), CyclesArr[1].end());

	return Temp;
}
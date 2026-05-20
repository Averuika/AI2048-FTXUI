#ifndef AI2048_GAME_HPP
#define AI2048_GAME_HPP

#include <array>
#include <random>
#include <vector>

namespace ai2048 {

constexpr int kBoardSize = 4;
using Board = std::array<std::array<int, kBoardSize>, kBoardSize>;

enum class Direction {
  Up,
  Down,
  Left,
  Right,
};

class Game2048 {
 public:
  Game2048();

  void Reset();
  bool Move(Direction direction);

  const Board& board() const { return board_; }
  int score() const { return score_; }
  int move_count() const { return move_count_; }
  bool won() const { return won_; }
  bool game_over() const { return game_over_; }

  int LargestTile() const;
  bool HasEmptyCell() const;
  bool AnyMoveAvailable() const;

 private:
  Board board_{};
  int score_ = 0;
  int move_count_ = 0;
  bool won_ = false;
  bool game_over_ = false;
  std::mt19937 rng_;

  void AddRandomTile();
  static std::vector<int> SlideAndMerge(const std::vector<int>& line, int& gained_score);
  static bool SameBoard(const Board& lhs, const Board& rhs);
};

}  // namespace ai2048

#endif  // AI2048_GAME_HPP

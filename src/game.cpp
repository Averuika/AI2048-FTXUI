#include "game.hpp"

#include <algorithm>
#include <chrono>

namespace ai2048 {

Game2048::Game2048()
    : rng_(static_cast<unsigned int>(
          std::chrono::steady_clock::now().time_since_epoch().count())) {
  Reset();
}

void Game2048::Reset() {
  for (auto& row : board_) {
    row.fill(0);
  }
  score_ = 0;
  move_count_ = 0;
  won_ = false;
  game_over_ = false;
  AddRandomTile();
  AddRandomTile();
}

std::vector<int> Game2048::SlideAndMerge(const std::vector<int>& line,
                                         int& gained_score) {
  std::vector<int> compact;
  compact.reserve(kBoardSize);

  for (int value : line) {
    if (value != 0) {
      compact.push_back(value);
    }
  }

  std::vector<int> merged;
  merged.reserve(kBoardSize);
  for (std::size_t i = 0; i < compact.size(); ++i) {
    if (i + 1 < compact.size() && compact[i] == compact[i + 1]) {
      const int new_value = compact[i] * 2;
      merged.push_back(new_value);
      gained_score += new_value;
      ++i;
    } else {
      merged.push_back(compact[i]);
    }
  }

  while (merged.size() < kBoardSize) {
    merged.push_back(0);
  }
  return merged;
}

bool Game2048::Move(Direction direction) {
  if (game_over_) {
    return false;
  }

  Board next = board_;
  int gained_score = 0;

  for (int fixed = 0; fixed < kBoardSize; ++fixed) {
    std::vector<int> line;
    line.reserve(kBoardSize);

    switch (direction) {
      case Direction::Left:
        for (int col = 0; col < kBoardSize; ++col) {
          line.push_back(board_[fixed][col]);
        }
        break;
      case Direction::Right:
        for (int col = kBoardSize - 1; col >= 0; --col) {
          line.push_back(board_[fixed][col]);
        }
        break;
      case Direction::Up:
        for (int row = 0; row < kBoardSize; ++row) {
          line.push_back(board_[row][fixed]);
        }
        break;
      case Direction::Down:
        for (int row = kBoardSize - 1; row >= 0; --row) {
          line.push_back(board_[row][fixed]);
        }
        break;
    }

    const std::vector<int> merged = SlideAndMerge(line, gained_score);

    switch (direction) {
      case Direction::Left:
        for (int col = 0; col < kBoardSize; ++col) {
          next[fixed][col] = merged[col];
        }
        break;
      case Direction::Right:
        for (int col = kBoardSize - 1, i = 0; col >= 0; --col, ++i) {
          next[fixed][col] = merged[i];
        }
        break;
      case Direction::Up:
        for (int row = 0; row < kBoardSize; ++row) {
          next[row][fixed] = merged[row];
        }
        break;
      case Direction::Down:
        for (int row = kBoardSize - 1, i = 0; row >= 0; --row, ++i) {
          next[row][fixed] = merged[i];
        }
        break;
    }
  }

  if (SameBoard(board_, next)) {
    game_over_ = !AnyMoveAvailable();
    return false;
  }

  board_ = next;
  score_ += gained_score;
  ++move_count_;
  AddRandomTile();
  won_ = LargestTile() >= 2048;
  game_over_ = !AnyMoveAvailable();
  return true;
}

void Game2048::AddRandomTile() {
  std::vector<std::pair<int, int>> empty_cells;
  for (int row = 0; row < kBoardSize; ++row) {
    for (int col = 0; col < kBoardSize; ++col) {
      if (board_[row][col] == 0) {
        empty_cells.emplace_back(row, col);
      }
    }
  }

  if (empty_cells.empty()) {
    return;
  }

  std::uniform_int_distribution<std::size_t> cell_dist(0, empty_cells.size() - 1);
  std::uniform_int_distribution<int> value_dist(1, 10);
  const auto [row, col] = empty_cells[cell_dist(rng_)];
  board_[row][col] = (value_dist(rng_) == 1) ? 4 : 2;
}

int Game2048::LargestTile() const {
  int largest = 0;
  for (const auto& row : board_) {
    largest = std::max(largest, *std::max_element(row.begin(), row.end()));
  }
  return largest;
}

bool Game2048::HasEmptyCell() const {
  for (const auto& row : board_) {
    for (int value : row) {
      if (value == 0) {
        return true;
      }
    }
  }
  return false;
}

bool Game2048::AnyMoveAvailable() const {
  if (HasEmptyCell()) {
    return true;
  }

  for (int row = 0; row < kBoardSize; ++row) {
    for (int col = 0; col < kBoardSize; ++col) {
      const int value = board_[row][col];
      if (row + 1 < kBoardSize && board_[row + 1][col] == value) {
        return true;
      }
      if (col + 1 < kBoardSize && board_[row][col + 1] == value) {
        return true;
      }
    }
  }
  return false;
}

bool Game2048::SameBoard(const Board& lhs, const Board& rhs) {
  for (int row = 0; row < kBoardSize; ++row) {
    for (int col = 0; col < kBoardSize; ++col) {
      if (lhs[row][col] != rhs[row][col]) {
        return false;
      }
    }
  }
  return true;
}

}  // namespace ai2048

#include "game.hpp"

#include <locale>
#include <string>
#include <utility>

#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

namespace {

Color TileBackground(int value) {
  switch (value) {
    case 0:
      return Color::RGB(34, 39, 46);
    case 2:
      return Color::RGB(238, 228, 218);
    case 4:
      return Color::RGB(237, 224, 200);
    case 8:
      return Color::RGB(242, 177, 121);
    case 16:
      return Color::RGB(245, 149, 99);
    case 32:
      return Color::RGB(246, 124, 95);
    case 64:
      return Color::RGB(246, 94, 59);
    case 128:
      return Color::RGB(237, 207, 114);
    case 256:
      return Color::RGB(237, 204, 97);
    case 512:
      return Color::RGB(237, 200, 80);
    case 1024:
      return Color::RGB(237, 197, 63);
    case 2048:
      return Color::RGB(237, 194, 46);
    default:
      return Color::RGB(60, 58, 50);
  }
}

Color TileForeground(int value) {
  if (value == 0) {
    return Color::GrayDark;
  }
  if (value <= 4) {
    return Color::RGB(119, 110, 101);
  }
  return Color::White;
}

Element Tile(int value) {
  const std::string label = value == 0 ? " " : std::to_string(value);
  return text(label) | bold | center | size(WIDTH, EQUAL, 8) |
         size(HEIGHT, EQUAL, 3) | borderRounded | color(TileForeground(value)) |
         bgcolor(TileBackground(value));
}

Element BoardView(const ai2048::Game2048& game) {
  Elements rows;
  const auto& board = game.board();
  for (int row = 0; row < ai2048::kBoardSize; ++row) {
    Elements cells;
    for (int col = 0; col < ai2048::kBoardSize; ++col) {
      cells.push_back(Tile(board[row][col]));
    }
    rows.push_back(hbox(std::move(cells)));
  }
  return vbox(std::move(rows)) | borderHeavy | bgcolor(Color::RGB(26, 31, 38));
}

Element StatBox(const std::string& title, const std::string& value) {
  return vbox({text(title) | dim | center, text(value) | bold | center}) |
         size(WIDTH, EQUAL, 14) | borderRounded;
}

Element HelpBox() {
  return vbox({
             text("控制说明") | bold,
             separator(),
             text("↑ ↓ ← → / W A S D : 移动"),
             text("R : 重新开始"),
             text("Q / Esc : 退出"),
             separator(),
             text("规则：相同数字相撞会合并，"),
             text("目标是合成 2048。"),
         }) |
         borderRounded | size(WIDTH, EQUAL, 38);
}

Element StatusLine(const ai2048::Game2048& game) {
  if (game.game_over()) {
    return text("游戏结束：没有可移动方向。按 R 重新开始。") | bold |
           color(Color::RedLight);
  }
  if (game.won()) {
    return text("你已经合成 2048！可以继续挑战更高分。") | bold |
           color(Color::YellowLight);
  }
  return text("正在游戏：移动方块，合并相同数字。") | color(Color::GreenLight);
}

Element Render(const ai2048::Game2048& game) {
  return vbox({
             text("AI 2048 - FTXUI Terminal Game") | bold | center |
                 color(Color::CyanLight),
             separator(),
             hbox({
                 BoardView(game),
                 separatorEmpty(),
                 vbox({
                     hbox({StatBox("Score", std::to_string(game.score())),
                           StatBox("Moves", std::to_string(game.move_count()))}),
                     hbox({StatBox("Best Tile", std::to_string(game.LargestTile())),
                           StatBox("Goal", "2048")}),
                     separatorEmpty(),
                     HelpBox(),
                 }),
             }),
             separator(),
             StatusLine(game) | center,
         }) |
         border | bgcolor(Color::RGB(15, 18, 23));
}

bool HandleMoveKey(const Event& event, ai2048::Game2048& game) {
  if (event == Event::ArrowLeft || event == Event::Character("a") ||
      event == Event::Character("A")) {
    game.Move(ai2048::Direction::Left);
    return true;
  }
  if (event == Event::ArrowRight || event == Event::Character("d") ||
      event == Event::Character("D")) {
    game.Move(ai2048::Direction::Right);
    return true;
  }
  if (event == Event::ArrowUp || event == Event::Character("w") ||
      event == Event::Character("W")) {
    game.Move(ai2048::Direction::Up);
    return true;
  }
  if (event == Event::ArrowDown || event == Event::Character("s") ||
      event == Event::Character("S")) {
    game.Move(ai2048::Direction::Down);
    return true;
  }
  return false;
}

}  // namespace

int main() {
  std::locale::global(std::locale(""));

  ai2048::Game2048 game;
  auto screen = ScreenInteractive::TerminalOutput();

  auto renderer = Renderer([&] { return Render(game); });
  auto component = CatchEvent(renderer, [&](Event event) {
    if (HandleMoveKey(event, game)) {
      return true;
    }
    if (event == Event::Character("r") || event == Event::Character("R")) {
      game.Reset();
      return true;
    }
    if (event == Event::Character("q") || event == Event::Character("Q") ||
        event == Event::Escape) {
      screen.ExitLoopClosure()();
      return true;
    }
    return false;
  });

  screen.Loop(component);
  return 0;
}

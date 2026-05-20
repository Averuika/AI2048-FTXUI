# AI 2048 - FTXUI 终端小游戏

这是一个基于 **C++17** 和 **FTXUI** 开发的 2048 终端界面小游戏。项目参考了 `Dashark/Tetris-mini-game` 仓库的工程组织思路：使用 `src/` 存放源代码，用 `CMakeLists.txt` 构建项目，并通过 FTXUI 实现字符终端下的游戏界面。本项目没有复制该仓库的俄罗斯方块源代码，而是重新实现了 2048 的核心逻辑与界面。

## 项目功能

- 4 × 4 标准 2048 棋盘。
- 支持方向键和 `W A S D` 移动。
- 相同数字合并并累计分数。
- 自动随机生成新数字块，默认生成 `2`，小概率生成 `4`。
- 显示当前分数、移动次数、最大数字块和游戏状态。
- 支持合成 2048 后继续挑战。
- 支持游戏结束提示与重新开始。
- 使用 FTXUI 绘制终端 GUI。

## 操作方法

| 按键 | 功能 |
| --- | --- |
| `↑` / `W` | 向上移动 |
| `↓` / `S` | 向下移动 |
| `←` / `A` | 向左移动 |
| `→` / `D` | 向右移动 |
| `R` | 重新开始 |
| `Q` / `Esc` | 退出游戏 |

## 构建要求

| 工具 | 最低要求 |
| --- | --- |
| CMake | 3.20 |
| C++ 编译器 | 支持 C++17，例如 GCC 8+、Clang 7+、MSVC 2019+ |

FTXUI 会优先使用本机已安装的 FTXUI；如果没有安装，则通过 CMake `FetchContent` 自动下载。第一次配置项目时需要能访问 GitHub。

## 编译与运行

### Windows PowerShell / Developer Command Prompt

```powershell
cmake -B build
cmake --build build --config Release
.\build\Release\ai_2048.exe
```

如果你使用的是 MinGW 或 Ninja，也可以运行：

```powershell
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
.\build\ai_2048.exe
```

### macOS / Linux

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/ai_2048
```

### Debug + AddressSanitizer，可选

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON
cmake --build build
./build/ai_2048
```

## 项目结构

```text
AI2048-FTXUI/
├── CMakeLists.txt        # CMake 构建脚本，自动拉取 FTXUI
├── README.md             # 项目说明、使用方法、贡献指南
├── .gitignore            # 忽略构建产物和 IDE 临时文件
└── src/
    ├── game.hpp          # 2048 游戏逻辑类声明
    ├── game.cpp          # 棋盘移动、合并、计分、随机生成逻辑
    └── main.cpp          # FTXUI 界面渲染和键盘事件处理
```

## 2048 规则说明

1. 棋盘大小为 4 × 4。
2. 每次移动时，所有数字块会沿指定方向滑动。
3. 同一行或同一列中，相邻且数值相同的数字块会合并为一个更大的数字块。
4. 每次有效移动后，系统会在空格子中随机生成一个新的数字块。
5. 当棋盘中出现 `2048` 时，玩家达成目标。
6. 当棋盘已满且上下左右都无法合并时，游戏结束。

## AI 辅助开发说明

本项目按照课程作业要求由 AI 辅助生成，主要包括：

- 根据任务要求设计 C++17 项目结构。
- 生成 2048 核心游戏逻辑。
- 生成 FTXUI 终端界面代码。
- 生成 CMake 构建配置。
- 生成 README 文档与贡献指南。

参考开源仓库：

- `Dashark/Tetris-mini-game`：参考其使用 FTXUI 构建终端游戏的思路、CMake 项目组织方式和 README 写法。
- `FTXUI`：用于构建 C++ 终端图形界面。

原创性声明：本项目的游戏类型、核心逻辑和源代码均为重新实现的 2048 游戏，没有直接抄袭参考仓库的俄罗斯方块源码。

## 贡献指南

如果要继续完善本项目，建议按照以下方式进行：

1. 在个人 GitHub 账号中新建仓库，不要直接在参考仓库中提交。
2. 将本项目文件上传到自己的仓库。
3. 修改前新建分支，例如：

```bash
git checkout -b feature/improve-ui
```

4. 保持代码结构清晰：
   - 游戏逻辑放在 `src/game.hpp` 和 `src/game.cpp`。
   - 界面渲染与键盘事件放在 `src/main.cpp`。
   - 不要把编译生成的 `build/`、`.exe` 等文件提交到仓库。
5. 提交时写清楚修改内容，例如：

```bash
git add .
git commit -m "Add 2048 game with FTXUI interface"
git push origin feature/improve-ui
```

## 可继续改进方向

- 添加历史最高分保存功能。
- 添加撤销一步功能。
- 添加不同主题颜色。
- 添加开始菜单和难度模式。
- 添加更完整的单元测试。

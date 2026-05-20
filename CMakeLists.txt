cmake_minimum_required(VERSION 3.20)

project(AI2048FTXUI
    VERSION 1.0.0
    DESCRIPTION "AI generated 2048 game built with C++17 and FTXUI"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_ASAN "Enable AddressSanitizer for GCC/Clang debug builds" OFF)

find_package(ftxui QUIET)

if(NOT ftxui_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
        GIT_TAG v5.0.0
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(ftxui)
endif()

add_executable(ai_2048
    src/main.cpp
    src/game.cpp
    src/game.hpp
)

target_include_directories(ai_2048 PRIVATE src)
target_link_libraries(ai_2048
    PRIVATE
        ftxui::screen
        ftxui::dom
        ftxui::component
)

if(MSVC)
    target_compile_options(ai_2048 PRIVATE /W4 /permissive-)
else()
    target_compile_options(ai_2048 PRIVATE -Wall -Wextra -Wpedantic)
    if(ENABLE_ASAN)
        target_compile_options(ai_2048 PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(ai_2048 PRIVATE -fsanitize=address)
    endif()
endif()

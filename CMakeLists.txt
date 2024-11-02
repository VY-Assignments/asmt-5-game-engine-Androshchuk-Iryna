cmake_minimum_required(VERSION 3.28)
project(asmt_5_game_engine_Androshchuk_Iryna)

set(CMAKE_CXX_STANDARD 14)

add_executable(asmt_5_game_engine_Androshchuk_Iryna main.cpp)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

target_link_libraries(asmt_5_game_engine_Androshchuk_Iryna PRIVATE sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

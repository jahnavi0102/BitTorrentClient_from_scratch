cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(BitTorrentClient)

add_executable(BitTorrentClient main.cpp)

target_include_directories(BitTorrentClient PRIVATE ${CMAKE_SOURCE_DIR}/include)


cmake_minimum_required(VERSION 3.10)

project(authSystemRedis)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
  main.cpp
  db.cpp
)

add_executable(authSystemRedis ${SOURCES})

find_path(HIREDIS_HEADER hiredis)
target_include_directories(authSystemRedis PUBLIC ${HIREDIS_HEADER})

find_library(HIREDIS_LIB hiredis)
target_link_libraries(authSystemRedis ${HIREDIS_LIB})

find_path(REDIS_PLUS_PLUS_HEADER sw)
target_include_directories(authSystemRedis PUBLIC ${REDIS_PLUS_PLUS_HEADER})

find_library(REDIS_PLUS_PLUS_LIB redis++)
target_link_libraries(authSystemRedis ${REDIS_PLUS_PLUS_LIB})

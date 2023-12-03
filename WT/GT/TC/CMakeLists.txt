cmake_minimum_required(VERSION 3.27)

# set the project name
project(TC)
SET(SRC_LIST main.c)

# add the executable
add_executable(${PROJECT_NAME} ${SRC_LIST})

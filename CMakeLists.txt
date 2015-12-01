cmake_minimum_required(VERSION 3.3)
project(ub_sistemes_operatius_2)
find_package(Threads)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")

set(SOURCE_FILES
    extract-words.c
    extract-words.h
    graph.c
    graph.h
    hashutils.c
    hashutils.h
    linked-list.c
    linked-list.h
    Makefile
    menu.c
    red-black-tree.c
    red-black-tree.h
    strutils.c
    strutils.h
    treeio.c
    treeio.h)

add_executable(ub_sistemes_operatius_2 ${SOURCE_FILES})
target_link_libraries(ub_sistemes_operatius_2 ${CMAKE_THREAD_LIBS_INIT})
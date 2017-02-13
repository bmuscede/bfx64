cmake_minimum_required(VERSION 3.6)
project(bfx64)

find_package(Boost 1.58 COMPONENTS system filesystem program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

set(LLVM_BUILD_PATH /home/bmuscede/Applications/LLVM-Build)
link_directories(${LLVM_BUILD_PATH}/lib)
include_directories(${LLVM_BUILD_PATH}/include)

add_definitions(
        -D__STDC_LIMIT_MACROS
        -D__STDC_CONSTANT_MACROS
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        main.cpp
        ELF/ElfReader.cpp
        ELF/ElfReader.h
        Graph/TAGraph.cpp
        Graph/TAGraph.h
        Graph/BFXNode.cpp
        Graph/BFXNode.h
        Graph/BFXEdge.cpp
        Graph/BFXEdge.h
        TAFunctions.h
        TAFunctions.cpp
        Print/PrintOperation.cpp Print/PrintOperation.h Print/ProgressBar.c Print/ProgressBar.h)

add_executable(bfx64 ${SOURCE_FILES})

include(FindCurses)
target_link_libraries(bfx64
        ${Boost_LIBRARIES}
        ${CURSES_LIBRARIES})
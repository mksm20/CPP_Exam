cmake_minimum_required(VERSION 3.27)
project(Exam_Lib)

set(CMAKE_CXX_STANDARD 23)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/matplotlibcpp)

# Find Python
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Manually get the NumPy include directory using Python
execute_process(
        COMMAND ${Python3_EXECUTABLE} -c "import numpy; print(numpy.get_include())"
        OUTPUT_VARIABLE Python3_NumPy_INCLUDE_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Include NumPy directory
include_directories(${Python3_NumPy_INCLUDE_DIR})

# Add executable
add_executable(Exam_Lib
        main.cpp
        Species.cpp
        Reaction.cpp
        SystemState.cpp
        Simulator.cpp
        SymbolTable.cpp
)

# Link libraries
target_link_libraries(Exam_Lib Python3::Python)

cmake_minimum_required(VERSION 3.27)
project(Exam_Lib)

set(CMAKE_CXX_STANDARD 23)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/matplotlibcpp)

include_directories(${GRAPHVIZ_INCLUDE_DIRS})

# Find Graphviz
find_package(PkgConfig REQUIRED)
pkg_check_modules(GRAPHVIZ REQUIRED libgvc)

# Add the required libraries
include_directories(${GRAPHVIZ_INCLUDE_DIRS})
link_directories(${GRAPHVIZ_LIBRARY_DIRS})
add_definitions(${GRAPHVIZ_CFLAGS_OTHER})

# Find Python
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Manually get the NumPy include directory using Python
execute_process(
        COMMAND ${Python3_EXECUTABLE} -c "import numpy; print(numpy.get_include())"
        OUTPUT_VARIABLE Python3_NumPy_INCLUDE_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Add debugging flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})




# Include NumPy directory
include_directories(${Python3_NumPy_INCLUDE_DIR})

# Add executable
add_executable(Exam_Lib
        main.cpp
        ../src/Species.cpp
        ../src/Reaction.cpp
        ../src/SystemState.cpp
        ../src/Simulator.cpp
        ../src/Vessel.cpp
        ../Vessel.h
        ../src/GraphVisualizer.cpp
        GraphVisualizer.h
        PeakHospitalizationObserver.h
)
add_executable(runUnitTests
        ../test_main.cpp
        ../test_symbol_table.cpp
)
# Benchmark executable
add_executable(runBenchmarks
        ../benchmark.cpp
        ../src/Reaction.cpp
        ../src/Species.cpp
        ../src/SystemState.cpp
        ../src/Simulator.cpp
        ../src/Vessel.cpp
)
target_link_libraries(runBenchmarks pthread)
target_link_libraries(runUnitTests ${GTEST_LIBRARIES} pthread)
add_test(NAME runUnitTests COMMAND runUnitTests)


# Link libraries
target_link_libraries(Exam_Lib ${GRAPHVIZ_LIBRARIES})
target_link_libraries(Exam_Lib Python3::Python)

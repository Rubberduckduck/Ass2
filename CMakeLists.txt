cmake_minimum_required(VERSION 3.10)
project(SeamCarving)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Explicitly set OpenCV path (adjust if your path is different)
set(OpenCV_DIR "C:/opencv/build")

# Find OpenCV - try different versions
find_package(OpenCV 4 QUIET)
if(NOT OpenCV_FOUND)
    find_package(OpenCV 3 QUIET)
endif()
if(NOT OpenCV_FOUND)
    find_package(OpenCV REQUIRED)
endif()

message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")

include_directories(${OpenCV_INCLUDE_DIRS})

# Create executable
add_executable(seam_carving seam_carving.cpp)

# Link OpenCV libraries
target_link_libraries(seam_carving ${OpenCV_LIBS})

# Optional: Set compiler flags for optimization
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        target_compile_options(seam_carving PRIVATE /O2)
    else()
        target_compile_options(seam_carving PRIVATE -O3)
    endif()
endif()
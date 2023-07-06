cmake_minimum_required (VERSION 3.11)
 
# --- Fetch libs --------------------------------------------------------------

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(std-k
  GIT_REPOSITORY https://github.com/kiddersmxj/std-k
  GIT_TAG main
)
 
FetchContent_GetProperties(std-k)
if(NOT std-k_POPULATED)
  FetchContent_Populate(std-k)
  add_subdirectory(${std-k_SOURCE_DIR} ${std-k_BINARY_DIR})
endif()
 
# ------------------------------------------------------------------------------

# Set cpp standard (with flags for gh actions)
# set (CMAKE_CXX_STANDARD 20)
# set(CXXFLAGS "-std=c++20")
# set(CMAKE_CXX_FLAGS "${CXXFLAGS}")

project(create
  LANGUAGES CXX
  VERSION 0.0.0
)

add_executable(create src/create.cpp)

target_include_directories(create
    PRIVATE src
    SHARED ${std-k_SOURCE_DIR}/inc
)

install(
    TARGETS create
    DESTINATION /usr/local/bin/
)

link_directories(${std-k_BINARY_DIR})
target_link_libraries(create
    PUBLIC std-k
)
target_compile_features(create PUBLIC cxx_std_20)

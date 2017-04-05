cmake_minimum_required(VERSION 3.6)
project(tff CXX)

set(CMAKE_CXX_STANDARD 14)

include_directories(SYSTEM external/include)
add_compile_options(-Wall -Wextra -Wno-sign-compare)

# library
file(GLOB_RECURSE TFF_SRC "src/*.cc" "src/*.h" "src/*.tcc" "src/*.icc")
add_library(tff SHARED ${TFF_SRC})
target_compile_definitions(tff PRIVATE BACKWARD_HAS_BFD=1)
target_link_libraries(tff pthread atomic bfd)

# tests
file(GLOB_RECURSE TFF_TEST_SRC "test/*.cc" "test/*.h" "test/*.tcc" "test/*.icc")
add_executable(tff_test ${TFF_TEST_SRC})
target_include_directories(tff_test PRIVATE src)
target_link_libraries(tff_test tff)
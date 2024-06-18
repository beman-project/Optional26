include_guard(GLOBAL)

set(CMAKE_C_COMPILER clang-17)
set(CMAKE_CXX_COMPILER clang++-17)

include("${CMAKE_CURRENT_LIST_DIR}/clang-flags.cmake")

set(CMAKE_CXX_FLAGS
   "-stdlib=libstdc++ -Wall -Wextra "
CACHE STRING "CXX_FLAGS" FORCE)

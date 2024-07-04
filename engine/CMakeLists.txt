add_library(Engine_lib SHARED src/core/logger.cpp src/core/logger.hpp src/defines.hpp)
target_include_directories(Engine_lib PRIVATE src/)
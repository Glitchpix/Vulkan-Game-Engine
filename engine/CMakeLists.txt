add_library(Engine_lib SHARED src/core/logger.cpp src/core/logger.hpp
                              src/core/asserts.hpp src/defines.hpp 
                              src/platform/platform.hpp src/platform/platform_win32.cpp
                              src/core/application.hpp src/core/application.cpp)
target_include_directories(Engine_lib PRIVATE src/)
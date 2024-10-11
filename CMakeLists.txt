cmake_minimum_required(VERSION 3.29)
project(ScreenSnake2 VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find raylib package if installed via a package manager (e.g. Homebrew, apt, etc.)
find_package(raylib REQUIRED)

# Add your executable (replace main.cpp with your source files)
add_executable(ScreenSnake2 src/main.cpp
        src/Snake.cpp
        src/Snake.h
        src/SnakeSegment.cpp
        src/SnakeSegment.h
        src/Vector2i.cpp
        src/Vector2i.h
        src/Board.cpp
        src/Board.h
        src/Direction.h
        src/Food.cpp
        src/Food.h
)

# Link raylib to your project
target_link_libraries(ScreenSnake2 raylib)

# Optionally include raylib's header directories (in case you use functions like "raylib.h")
target_include_directories(ScreenSnake2 PUBLIC ${RAYLIB_INCLUDE_DIRS})

# For macOS: If you installed raylib via Homebrew, you may need to link additional frameworks
if(APPLE)
    target_link_libraries(ScreenSnake2 "-framework CoreVideo")
    target_link_libraries(ScreenSnake2 "-framework IOKit")
    target_link_libraries(ScreenSnake2 "-framework Cocoa")
    target_link_libraries(ScreenSnake2 "-framework GLUT")
    target_link_libraries(ScreenSnake2 "-framework OpenGL")
endif()


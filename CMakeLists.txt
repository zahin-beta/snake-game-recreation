cmake_minimum_required(VERSION 3.15)
project(snake-game C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ---- raylib dependency ----
if(NOT DEFINED RAYLIB_DIR)
    set(RAYLIB_DIR "D:/c project/raylib" CACHE PATH "Path to raylib root")
endif()

set(RAYLIB_INCLUDE_DIR "${RAYLIB_DIR}/include")
set(RAYLIB_LIB_DIR     "${RAYLIB_DIR}/lib")

# ---- source files ----
set(SOURCES
    src/main.c
    src/game.c
    src/snake.c
    src/food.c
    src/collision.c
    src/obstacle.c
    src/raylib_api.c
    src/config.c
    src/log.c
    src/score.c
    src/assets.c
)

add_executable(game ${SOURCES})
target_include_directories(game PRIVATE src ${RAYLIB_INCLUDE_DIR})
target_link_libraries(game PRIVATE opengl32 gdi32 winmm)

# ---- tests ----
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    enable_testing()

    # Unity test framework (single-header, bundled)
    set(UNITY_HEADER "${CMAKE_SOURCE_DIR}/test/unity.h")

    add_executable(test_collision test/test_collision.c src/collision.c)
    target_include_directories(test_collision PRIVATE src ${RAYLIB_INCLUDE_DIR})

    add_test(NAME collision COMMAND test_collision)
endif()

# ---- install ----
install(TARGETS game RUNTIME DESTINATION bin)

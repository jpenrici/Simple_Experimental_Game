cmake_minimum_required(VERSION 3.5)

project(Game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    game.h game.cpp
    glfw_game.h
    sfml_game.h
    terminal_game.h
)

set(RESOURCE_FILES
    resources/levels/level_1.lvl
    resources/levels/level_2.lvl
    resources/fonts/NotoSans-Black.ttf
)

add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
)

add_subdirectory(glad/)

find_package (SFML 2.5 COMPONENTS audio graphics network system window REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(glfw3 REQUIRED)

include_directories(
    ${OPENGL_INCLUDE_DIRS}
)

target_link_libraries (
    ${PROJECT_NAME} PUBLIC
    ${OPENGL_LIBRARIES}
    sfml-audio
    sfml-graphics
    sfml-network
    sfml-system
    sfml-window
    glad
    glfw
)

file(COPY ${RESOURCE_FILES} DESTINATION ${CMAKE_BINARY_DIR}/Resources)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

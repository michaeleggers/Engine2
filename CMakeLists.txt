cmake_minimum_required(VERSION 3.31.6)

project(Engine2 VERSION 0.1.1)


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GLEW)
find_package(SDL2)
find_package(SDL2_image)

add_executable(Engine2
    ./Engine2/main.cpp
    ./Engine2/Mesh.cpp
    ./Engine2/Light.cpp
    ./Engine2/Camera.cpp
    ./Engine2/Weapon.cpp
    ./Engine2/Texture.cpp
    ./Engine2/Character.cpp
    ./Engine2/md2Model.cpp
    ./Engine2/md2Loader.cpp
    ./Engine2/display.cpp
    ./Engine2/BoundingBox.cpp
    ./Engine2/Entity.cpp
    ./Engine2/AnimatedEntity.cpp
    ./Engine2/AnimationShader.cpp
    ./Engine2/AnimatedBoundingBox.cpp
)

target_include_directories(Engine2 PUBLIC
    ./Engine2/include/
)

target_link_libraries(Engine2
    PUBLIC -lc
    PUBLIC -lm
    PRIVATE SDL2::SDL2
    PRIVATE SDL2_image::SDL2_image
    PRIVATE GLEW::GLEW
)



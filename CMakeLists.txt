cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project("readSTL")
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
add_subdirectory(ext/glm)

add_executable(readSTL)
add_library(glad STATIC)

set_target_properties(readSTL
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

target_sources(readSTL PRIVATE
    "src/main.cpp"
    "src/world.cpp"
    "src/matrixStack.cpp"
    "src/STLmodel.cpp"
)
target_sources(glad PRIVATE
    "ext/glad/src/glad.c"
)

target_include_directories(readSTL PRIVATE
    "include/"
)
target_include_directories(glad PUBLIC
    "ext/glad/include"
)

target_link_libraries(readSTL PRIVATE 
    glad 
    glfw
    glm::glm
    OpenGL::GL
)

add_custom_command(TARGET readSTL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_CURRENT_SOURCE_DIR}/shader"
        "$<TARGET_FILE_DIR:readSTL>/shader"
    VERBATIM
)

add_custom_command(TARGET readSTL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_CURRENT_SOURCE_DIR}/textures"
        "$<TARGET_FILE_DIR:readSTL>/textures"
    VERBATIM
)

add_custom_command(TARGET readSTL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_CURRENT_SOURCE_DIR}/stl"
        "$<TARGET_FILE_DIR:readSTL>/stl"
    VERBATIM
)
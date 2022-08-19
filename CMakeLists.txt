cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project("HW2")
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
add_subdirectory(ext/glm)

add_executable(HW2)
add_library(glad STATIC)

set_target_properties(HW2
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

target_sources(HW2 PRIVATE
    "src/main.cpp"
    "src/world.cpp"
    "src/stb_image.cpp"
    "src/matrixStack.cpp"
    "src/STLmodel.cpp"
    "src/Texture.cpp"
)
target_sources(glad PRIVATE
    "ext/glad/src/glad.c"
)

target_include_directories(HW2 PRIVATE
    "include/"
)
target_include_directories(glad PUBLIC
    "ext/glad/include"
)

target_link_libraries(HW2 PRIVATE 
    glad 
    glfw
    glm::glm
    OpenGL::GL
)

add_custom_command(TARGET HW2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_CURRENT_SOURCE_DIR}/shader"
        "$<TARGET_FILE_DIR:HW2>/shader"
    VERBATIM
)

add_custom_command(TARGET HW2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_CURRENT_SOURCE_DIR}/textures"
        "$<TARGET_FILE_DIR:HW2>/textures"
    VERBATIM
)

add_custom_command(TARGET HW2 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_CURRENT_SOURCE_DIR}/stl"
        "$<TARGET_FILE_DIR:HW2>/stl"
    VERBATIM
)
cmake_minimum_required(VERSION 3.24)
if (WIN32)
set(CMAKE_TOOLCHAIN_FILE "C:/Program Files/Microsoft Visual Studio/18/Community/VC/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif ()
project(GtkOpenGL)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
file(GLOB HEADS ${CMAKE_SOURCE_DIR}/*.h)
file(GLOB SOURS ${CMAKE_SOURCE_DIR}/*.c)
file(GLOB GLSL ${CMAKE_SOURCE_DIR}/*.glsl)
file(COPY ${GLSL} DESTINATION ${CMAKE_BINARY_DIR})
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)
pkg_check_modules(EPOXY REQUIRED IMPORTED_TARGET epoxy)
add_executable(${PROJECT_NAME} ${HEADS} ${SOURS})
target_compile_definitions(${PROJECT_NAME} PRIVATE _USE_MATH_DEFINES)
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif ()
target_link_libraries(${PROJECT_NAME} PkgConfig::GTK4 PkgConfig::EPOXY ${OPENGL_LIBRARIES})
if (UNIX)
    target_link_libraries(${PROJECT_NAME} m)
endif ()

file(GLOB VERTS "${CMAKE_CURRENT_SOURCE_DIR}/*.vert")
file(GLOB FRAGS "${CMAKE_CURRENT_SOURCE_DIR}/*.frag")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    # 复制动态库文件（仅在源文件更新时复制）
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${VERTS} $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FRAGS} $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
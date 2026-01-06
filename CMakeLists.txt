# TODO: refactor cmakes in to folders

cmake_minimum_required(VERSION 3.15)

include(vcpkg/vcpkg.cmake)
project(MoonEscape3D VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/gui/core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/gui/namespace
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/utils/include
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/utils/namespace
)

file(GLOB_RECURSE SOURCES
    modules/entry_point/main.cpp
    modules/gui/core/src/*.cpp
    modules/utils/src/*.cpp
)

find_package(Qt6OpenGL CONFIG REQUIRED)
find_package(Qt6OpenGLWidgets CONFIG REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable(MoonEscape3D
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/gui/core/include/moon_escape_3D_widget.h # important for AUTOMOC
    ${SOURCES}
)

target_link_libraries(MoonEscape3D PRIVATE
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    Eigen3::Eigen
)

set_target_properties(MoonEscape3D PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

if (WIN32)
    # Path to windeployqt (vcpkg)
    set(WINDEPLOYQT_EXECUTABLE "$ENV{VCPKG_DIR}/installed/${VCPKG_TARGET_TRIPLET}/tools/Qt6/bin/windeployqt.exe")

    if (EXISTS "${WINDEPLOYQT_EXECUTABLE}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}" "$<TARGET_FILE:MoonEscape3D>"
            COMMENT "Running windeployqt for MoonEscape3D"
        )
    else()
        message(WARNING "windeployqt not found: ${WINDEPLOYQT_EXECUTABLE}")
    endif()
endif()

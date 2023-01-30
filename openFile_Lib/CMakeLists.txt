cmake_minimum_required(VERSION 3.21)

project(
    openFile_Lib
    VERSION 1.0
    DESCRIPTION "To just Open Files"
    LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(HOME_DIR /home/alfredo/QtProjects)

set(SRC_CODE
    include/openFile_Lib_global.h
    include/openFile_LibProj.h
    include/openFile_Lib.h
    src/openFile_Lib.cpp
)

LINK_DIRECTORIES(
    ${HOME_DIR}/build-errorCodeAndMsg_Lib-Desktop_Qt_6_2_2_GCC_64bit-Debug
)

add_library(
    ${PROJECT_NAME}
    SHARED
    ${SRC_CODE}
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    include
    ${HOME_DIR}/errorCodeAndMsg_Lib/include
    ${HOME_DIR}/errorCodeAndMsg_Lib/src
)

target_link_libraries(
    ${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    OPENFILE_LIB_LIBRARY
)

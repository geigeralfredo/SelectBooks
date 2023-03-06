cmake_minimum_required(VERSION 3.21)

project(
    errorCodeAndMsg_Lib
    VERSION 1.0
    DESCRIPTION "To return an object with an Error Code and its respective Error Message"
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
    include/errorCodeAndMsg_Lib_global.h
    include/errorCodeAndMsg_LibProj.h
    include/errorCodeAndMsg_Lib.h
    src/errorCodeAndMsg_Lib.cpp
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
    ${HOME_DIR}/openFile_Lib/include
)

target_link_libraries(
    ${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    ${HOME_DIR}/build-openFile_Lib-Desktop_Qt_6_2_2_GCC_64bit-Debug/libopenFile_Lib.so
)

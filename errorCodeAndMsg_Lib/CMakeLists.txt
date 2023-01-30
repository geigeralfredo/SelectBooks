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


LINK_DIRECTORIES(
    ${HOME_DIR}/build-openFile_Lib-Desktop_Qt_6_2_2_GCC_64bit-Debug
    ${HOME_DIR}/build-errorCodeAndMsg_Lib-Desktop_Qt_6_2_2_GCC_64bit-Debug
)

add_library(
    ${PROJECT_NAME}
    SHARED
    ${SRC_CODE}
)

target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC
        QT_NO_DEBUG_OUTPUT
        QT_NO_INFO_OUTPUT
        QT_NO_WARNING_OUTPUT
        )

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    include
    ../errorCodeAndMsg_Lib/include
    ../openFile_Lib/include
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    ERRORCODEANDMSG_LIB_LIBRARY
)

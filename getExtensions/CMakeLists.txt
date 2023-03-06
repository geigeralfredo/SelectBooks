cmake_minimum_required(VERSION 3.14)

project(getExtensions LANGUAGES CXX)

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
    include/getExtensionsProj.h
    src/main.cpp
)

add_executable(
    ${PROJECT_NAME}
    ${SRC_CODE}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    include/
    ../elapsedTime_Lib/
    ../errorCodeAndMsg_Lib/include
    ../openFile_Lib/include
)

target_link_libraries(
    ${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    ${HOME_DIR}/build-openFile_Lib-Desktop_Qt_6_2_2_GCC_64bit-Debug/libopenFile_Lib.so
    ${HOME_DIR}/build-errorCodeAndMsg_Lib-Desktop_Qt_6_2_2_GCC_64bit-Debug/liberrorCodeAndMsg_Lib.so
    ${HOME_DIR}/build-elapsedTime_Lib-Desktop_Qt_6_2_2_GCC_64bit-Debug/libelapsedTime_Lib.so
)
 

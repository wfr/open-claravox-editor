cmake_minimum_required(VERSION 3.14)

project(TestPresetSerialization LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt5 COMPONENTS Core Quick Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TestPresetSerialization tst_presetserialization.cpp)
add_test(NAME TestPresetSerialization COMMAND TestPresetSerialization)

target_link_libraries(TestPresetSerialization PRIVATE Qt${QT_VERSION_MAJOR}::Test)


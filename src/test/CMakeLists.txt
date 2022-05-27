cmake_minimum_required(VERSION 3.14)

project(TestPresetParser LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt5 COMPONENTS Core Quick Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TestPresetParser tst_presetparser.cpp ../backend/backend.qrc)
add_test(NAME TestPresetParser COMMAND TestPresetParser)

target_link_libraries(TestPresetParser PRIVATE Qt${QT_VERSION_MAJOR}::Test)

# Claravox back-end library
target_link_libraries (TestPresetParser LINK_PUBLIC claravox-backend)
target_include_directories (TestPresetParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../backend)

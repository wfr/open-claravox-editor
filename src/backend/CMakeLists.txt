cmake_minimum_required(VERSION 3.14)

project(claravox-backend LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(RTMIDI REQUIRED rtmidi)

add_library(claravox-backend STATIC
    Backend.cpp
    Claravox.cpp
    MIDIMessage.cpp
    MIDIParameter.cpp
    Parameters.cpp
    Preset.cpp
    PresetListModel.cpp
    TagListModel.cpp
    GroupListModel.cpp
    SortFilterPresetListModel.cpp
    Util.cpp
    Mod.cpp
    ModMatrix.cpp
    
    Backend.h
    Claravox.h
    MIDIMessage.h
    MIDIParameter.h
    Parameters.h
    Preset.h 
    PresetListModel.h
    TagListModel.h
    GroupListModel.h
    SortFilterPresetListModel.h
    Util.h
    Mod.h
    ModMatrix.h

    backend.qrc
)

target_link_libraries(claravox-backend PRIVATE Qt${QT_VERSION_MAJOR}::Core ${RTMIDI_LIBRARIES})
target_include_directories(claravox-backend PRIVATE ${RTMIDI_INCLUDE_DIRS})
target_compile_definitions(claravox-backend PRIVATE CLARAVOX_BACKEND_LIBRARY)

cmake_minimum_required(VERSION 3.16)

project(kak-qt VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
qt_standard_project_setup()

qt_add_executable(kak-qt
    src/rpc/line.cpp
    src/rpc/atom.cpp
    src/rpc/face.cpp
    src/rpc/color.cpp
    src/rpc/attribute.hpp
    src/rpc/rpc.cpp
    src/colorpalette.cpp
    src/keybindings.cpp
    src/drawoptions.cpp
    src/ipc.cpp
    src/container.cpp
    src/splitcontainer.cpp
    src/kakounesession.cpp
    src/kakouneclient.cpp
    src/kakounemenu.cpp
    src/kakouneinfobox.cpp
    src/kakounewidget.cpp
    src/kakounetextedit.cpp
    src/statusbar.cpp
    src/mainwindow.cpp
    src/main.cpp
)

target_link_libraries(kak-qt PRIVATE Qt6::Widgets Qt6::Network)

set_target_properties(kak-qt PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

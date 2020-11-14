cmake_minimum_required(VERSION 2.8)

project(copySearcher)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

add_executable(copySearcher main.cpp mainwindow.h mainwindow.cpp filecollector.h filecollector.cpp file_hash.cpp file_hash.h filedistributor.cpp filedistributor.h filecomparer.h filecomparer.cpp counter.cpp counter.h bigcomparer.cpp bigcomparer.h filedeleter.cpp filedeleter.h fileworker.cpp fileworker.h)
qt5_use_modules(copySearcher Widgets)

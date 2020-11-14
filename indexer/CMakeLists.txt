cmake_minimum_required(VERSION 2.8)

project(indexer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

find_package(Qt5Widgets REQUIRED)

add_executable(indexer main.cpp mainwindow.h mainwindow.cpp
        trigram.cpp trigram.h
        trigramcalcer.cpp trigramcalcer.h
        trigramworker.cpp trigramworker.h
        settings.cpp settings.h
        counter.cpp counter.h
        searcher.cpp searcher.h
        infilesearcher.cpp infilesearcher.h
        settingswindow.cpp settingswindow.h)

qt5_use_modules(indexer Widgets)

cmake_minimum_required(VERSION 3.1.0)

project(appveyor-cppunit-qt)

set(CMAKE_AUTOMOC ON)

#find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED Network REQUIRED)

SET(SOURCES main.cpp appveyor.cpp)

add_executable(appveyor-cppunit-qt ${SOURCES})
target_link_libraries(appveyor-cppunit-qt Qt5::Core Qt5::Network)


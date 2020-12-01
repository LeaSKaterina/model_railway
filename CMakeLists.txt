cmake_minimum_required(VERSION 3.15)
project(railway)

set(CMAKE_CXX_STANDARD 14)

add_executable(railway main.cpp classes/railway.cpp classes/railway.h
        classes/map.cpp classes/map.h classes/station.cpp classes/station.h
        classes/train.cpp classes/train.h  classes/van.cpp classes/van.h  classes/resource.cpp classes/resource.h classes/exception.h)
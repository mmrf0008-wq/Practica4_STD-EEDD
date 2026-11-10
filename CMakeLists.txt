cmake_minimum_required(VERSION 3.29)
project(Practica4_AVL_EEDD)

set(CMAKE_CXX_STANDARD 20)

add_executable(Practica4_AVL_EEDD main.cpp
        Parque.h
        ControlParques.h
        Parque.cpp
        ControlParques.cpp
        Arbol.cpp
        LectorCSV.cpp
        Especie.cpp)

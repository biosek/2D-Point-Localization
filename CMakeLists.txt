project(ADS2-LokalizaceBodu-zapoctak)
cmake_minimum_required(VERSION 2.8)
set(SRC_LIST
    main.cpp
    localization.h localization.cpp
    pavl.h pavl.cpp
    struktury.h)
add_executable(${PROJECT_NAME} ${SRC_LIST})

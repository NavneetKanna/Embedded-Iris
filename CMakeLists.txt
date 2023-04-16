cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 14)
project(Lab)

include(CTest)
enable_testing()

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)
find_package(GTest REQUIRED)
pkg_search_module(TESSERACT REQUIRED tesseract)
pkg_search_module(LEPTONICA REQUIRED lept)
pkg_search_module(ESPEAK REQUIRED espeak-ng)

include_directories(
  ${OpenCV_INCLUDE_DIRS}
  ${Tesseract_INCLUDE_DIRS}
  ${leptonica_INCLUDE_DIRS}
  ${espeak-ng_INCLUDE_DIRS}
  ${GTEST_INCLUDE_DIRS}
  ${/usr/local/include}
)

SET(LIBRARY_SOURCES
  main.cpp 
  ocr.cpp 
  od.cpp 
  obstacle_detection.cpp 
  speak.cpp
)
add_executable(embeddediris  main.cpp ocr.cpp od.cpp obstacle_detection.cpp speak.cpp)
#add_library(libb STATIC ${LIBRARY_SOURCES})

add_subdirectory(test)


find_library(WIRINGPI_LIBRARIES NAMES wiringPi)

#${Tesseract_LIBRARIES}
target_link_libraries(embeddediris
    ${OpenCV_LIBS}
    ${ESPEAK_LIBRARIES}
    tesseract
    ${Leptonica_LIBRARIES}
    ${WIRINGPI_LIBRARIES}
    #archive 
    Threads::Threads
)

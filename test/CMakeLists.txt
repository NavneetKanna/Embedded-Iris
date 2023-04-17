cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 14)
project(test)

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

find_library(WIRINGPI_LIBRARIES NAMES wiringPi)


#../ocr.cpp ../od.cpp ../obstacle_detection.cpp ../speak.cpp
add_executable(ocrtest TestOCR.cpp  ../src/ocr.cpp ../src/speak.cpp)
add_executable(odtest TestOD.cpp ../src/od.cpp ../src/speak.cpp)

#../ocr.cpp  ../obstacle_detection.cpp
#../od.cpp ../obstacle_detection.cpp


target_link_libraries(ocrtest
    ${OpenCV_LIBS}
    ${ESPEAK_LIBRARIES}
    tesseract
    ${Leptonica_LIBRARIES}
    ${WIRINGPI_LIBRARIES}
    gtest
    gtest_main
    #archive 
    Threads::Threads
)

target_link_libraries(odtest
    ${OpenCV_LIBS}
    ${ESPEAK_LIBRARIES}
    tesseract
    ${Leptonica_LIBRARIES}
    ${WIRINGPI_LIBRARIES}
    gtest
    gtest_main
    #archive 
    Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(ocrtest)
gtest_discover_tests(odtest)

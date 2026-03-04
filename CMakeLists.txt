cmake_minimum_required(VERSION 3.10)
project(PhotoEditor)

set(CMAKE_CXX_STANDARD 17)

# Ищем OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Добавляем исполняемый файл
add_executable(PhotoEditor src/main.cpp)

# Привязываем библиотеки OpenCV
target_link_libraries(PhotoEditor ${OpenCV_LIBRARIES})
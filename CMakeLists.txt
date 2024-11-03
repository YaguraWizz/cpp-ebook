# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Имя проекта
project(cpp-ebook)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем папку с исходниками
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

# Добавляем все исходные файлы из папки src
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")

# Создаем исполняемый файл из исходных файлов
add_executable(cpp-ebook ${SRC_FILES})

# Указываем папку для проектов Visual Studio или других сборочных файлов
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/project")

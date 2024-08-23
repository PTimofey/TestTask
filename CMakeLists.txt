cmake_minimum_required(VERSION 3.10)

# Название проекта
project(SFMLButtons)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Добавляем исполняемый файл
add_executable(SFMLButtons main.cpp)

# Найдем пакеты SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Добавляем библиотеки SFML к проекту
target_link_libraries(SFMLButtons sfml-graphics sfml-window sfml-system)

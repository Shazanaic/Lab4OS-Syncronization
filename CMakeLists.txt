# CMakeList.txt: проект CMake для Lab4OS-Sync; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("Lab4OS-Sync")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Lab4OS-Sync "receiver.cpp" "manager.h" "manager.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Lab4OS-Sync PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.

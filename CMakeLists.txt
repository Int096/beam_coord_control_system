# Минимальная версия CMake
cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

# Название проекта
project(Modeling_of_beam_coordinates_control_system)

# Поиск нужных пакетов
find_package(Geant4 REQUIRED ui_all vis_all)

# Подключение Geant-овских флагов компиляции
include(${Geant4_USE_FILE})

# Переменны-файлы с исходниками и заголовочными файлами
file(GLOB sources ${PROJECT_SOURCE_DIR}/sources/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/headers/*.hh)

# Перечисление файлов для компиляции
add_executable(Simulation simulation.cc ${sources} ${headers})

# Всякие таргеты
target_link_libraries(Simulation ${Geant4_LIBRARIES})
add_custom_target(Modeling_of_beam_coordinates_control_system DEPENDS Simulation)

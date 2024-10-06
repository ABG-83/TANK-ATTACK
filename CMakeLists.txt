cmake_minimum_required(VERSION 3.28.3)
project(TANK-ATTACK)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 14)

# Encontrar SDL2 y SDL2_image
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Agregar los archivos fuente
set(SOURCES
        main.cpp
        UI/Menu.cpp
        Game/Game.cpp
        Game/Grafo.cpp
)

# Agregar los archivos de encabezado
set(HEADERS
        UI/Menu.h
        Game/Game.h
        Game/Grafo.h
)

# Agregar el ejecutable
add_executable(tank_attack ${SOURCES} ${HEADERS})

# Enlazar las bibliotecas SDL2 y SDL2_image al ejecutable
target_link_libraries(tank_attack ${SDL2_LIBRARIES} SDL2 SDL2_ttf SDL2_image)

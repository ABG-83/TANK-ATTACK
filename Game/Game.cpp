#include "Game.h"
#include <iostream>

Game::Game(SDL_Renderer* renderer)
    : renderer(renderer), salir(false) {

    // Definir las dimensiones de la ventana
    int ventanaAncho = 800;
    int ventanaAlto = 600;

    // Establecer el número de filas y columnas
    int numFilas = 15;
    int numColumnas = 20;

    // Calcular el tamaño de cada celda
    ancho = ventanaAncho / numColumnas;
    alto = ventanaAlto / numFilas;

    // Crear el grafo con el mapa de la cuadrícula
    grafo = new Grafo(numColumnas, numFilas, ancho, alto);

    // Generar obstáculos de forma aleatoria
    grafo->generarObstaculos();
}


Game::~Game() {
    // Liberar el grafo
    delete grafo;
}

void Game::manejarEventos(SDL_Event* evento) {
    if (evento->type == SDL_QUIT) {
        salir = true;
    }
}

void Game::actualizar() {
    // actualizar el estado del juego, animaciones, etc.
}

void Game::renderizar() {
    // Limpiar la pantalla con un color de fondo
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Negro
    SDL_RenderClear(renderer);

    // Dibujar el grafo (cuadrícula y obstáculos)
    if (grafo != nullptr) {
        grafo->dibujarGrafo(renderer);
    } else {
        std::cout << "Error: El grafo no fue inicializado correctamente." << std::endl;
    }

    // Presentar el renderizado
    SDL_RenderPresent(renderer);
}

void Game::iniciar() {
    std::cout << "Iniciando el bucle del juego..." << std::endl;

    bool enJuego = true;
    SDL_Event evento;

    while (enJuego && !salir) {
        while (SDL_PollEvent(&evento)) {
            manejarEventos(&evento);
            std::cout << "Evento procesado." << std::endl;
        }

        actualizar();
        std::cout << "Estado del juego actualizado." << std::endl;

        renderizar();
        std::cout << "Pantalla renderizada." << std::endl;

        SDL_Delay(16); // para que la veocidad del bucle sean 60 fps mas o menos
    }

    std::cout << "Juego terminado." << std::endl;
}

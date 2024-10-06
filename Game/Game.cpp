#include "Game.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include "Grafo.h"

Game::Game(SDL_Renderer* renderer)
    : renderer(renderer), salir(false), tanqueSeleccionado(nullptr) {

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

    // Cargar las imagenes de los tanques
    tanqueRojo = IMG_LoadTexture(renderer, "/home/aldo-bagliano/Escritorio/TANK-ATTACK/Sprites/tanqueRojo.png");
    tanqueAzul = IMG_LoadTexture(renderer, "/home/aldo-bagliano/Escritorio/TANK-ATTACK/Sprites/tanqueAzul.png");
    tanqueAmarillo = IMG_LoadTexture(renderer, "/home/aldo-bagliano/Escritorio/TANK-ATTACK/Sprites/tanqueAmarillo.png");
    tanqueCeleste = IMG_LoadTexture(renderer, "/home/aldo-bagliano/Escritorio/TANK-ATTACK/Sprites/tanqueCeleste.png");

    if (!tanqueRojo || !tanqueAzul || !tanqueAmarillo || !tanqueCeleste) {
        std::cerr << "Error al cargar los sprites de los tanques: " << SDL_GetError() << std::endl;
        salir = true; // Si no se cargan los sprites, salir del juego
    }

    // Inicializar las posiciones iniciales de los tanques
    inicializarTanques();
}

Game::~Game() {
    // Liberar los recursos del grafo
    delete grafo;

    // Liberar texturas de los tanques
    SDL_DestroyTexture(tanqueRojo);
    SDL_DestroyTexture(tanqueAzul);
    SDL_DestroyTexture(tanqueAmarillo);
    SDL_DestroyTexture(tanqueCeleste);
}



void Game::inicializarTanques() {

    // Definir el número de tanques por jugador
    const int numTanquesPorJugador = 4;

    // Variables para guardar la posición del tanque
    int x, y;

    // Inicializar tanques del jugador 1 (rojo y azul)
    for (int i = 0; i < numTanquesPorJugador; ++i) {
        do {
            x = rand() % grafo->getNumColumnas();  // Generar coordenada X aleatoria
            y = rand() % grafo->getNumFilas();     // Generar coordenada Y aleatoria
        } while (!grafo->esCeldaLibre(x, y));       // Asegurarse de que la celda esté libre

        // Asignar color al tanque
        SDL_Texture* textura = (i < 2) ? tanqueRojo : tanqueAzul; // 2 tanques rojos, 2 tanques azules

        // Crear el tanque y añadirlo a la lista
        tanques.push_back(Tanque(x, y, ancho, alto, textura));
    }

    // Inicializar tanques del jugador 2 (amarillo y celeste)
    for (int i = 0; i < numTanquesPorJugador; ++i) {
        do {
            x = rand() % grafo->getNumColumnas();  // Generar coordenada X aleatoria
            y = rand() % grafo->getNumFilas();     // Generar coordenada Y aleatoria
        } while (!grafo->esCeldaLibre(x, y));       // Asegurarse de que la celda esté libre

        // Asignar color al tanque
        SDL_Texture* textura = (i < 2) ? tanqueAmarillo : tanqueCeleste; // 2 tanques amarillos, 2 celestes

        // Crear el tanque y añadirlo a la lista
        tanques.push_back(Tanque(x, y, ancho, alto, textura));
    }
}

void Game::manejarEventos(SDL_Event* evento) {
    if (evento->type == SDL_QUIT) {
        salir = true;
    }

    if (evento->type == SDL_MOUSEBUTTONDOWN && evento->button.button == SDL_BUTTON_LEFT) { // Verificar que es el clic izquierdo
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Convertir las coordenadas del mouse a la celda del mapa
        int celdaX = mouseX / ancho;
        int celdaY = mouseY / alto;

        // Verificar si se hizo clic en un tanque
        for (auto& tanque : tanques) {
            if (tanque.x == celdaX && tanque.y == celdaY) {
                tanqueSeleccionado = &tanque; // Seleccionar el tanque
                std::cout << "Tanque seleccionado en (" << tanque.x << ", " << tanque.y << ")" << std::endl;
                return; // Salir después de seleccionar el tanque
            }
        }

        // Si hay un tanque seleccionado, moverlo a la nueva posición
        if (tanqueSeleccionado != nullptr) {
            if (grafo->esCeldaLibre(celdaX, celdaY)) { // Verificar si la celda está libre
                tanqueSeleccionado->x = celdaX; // Mover el tanque a la nueva celda
                tanqueSeleccionado->y = celdaY;
                std::cout << "Tanque movido a (" << celdaX << ", " << celdaY << ")" << std::endl;
                tanqueSeleccionado = nullptr; // Desmarcar el tanque después de moverlo
            } else {
                std::cout << "Celda ocupada, no se puede mover el tanque." << std::endl;
            }
        }
    }
}



void Game::actualizar() {
    // Actualizar el estado del juego, animaciones, etc.
    if (tanqueSeleccionado != nullptr) {
        // Lógica para mover el tanque seleccionado u otras acciones
        std::cout << "Actualizando tanque seleccionado..." << std::endl;
    }
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

    // Renderizar los tanques
    for (const auto& tanque : tanques) {
        SDL_Rect dstRect = { tanque.x * ancho, tanque.y * alto, ancho, alto };
        SDL_RenderCopy(renderer, tanque.textura, nullptr, &dstRect); // Dibuja el sprite del tanque
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
        }

        actualizar();
        renderizar();

        SDL_Delay(16); // para que la velocidad del bucle sea 60 fps aproximadamente
    }

    std::cout << "Juego terminado." << std::endl;
}

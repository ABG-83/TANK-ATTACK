#include "Game.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include "Grafo.h"
#include <vector>
#include <SDL2/SDL_ttf.h>  // Para el texto


Game::Game(SDL_Renderer* renderer)
    : renderer(renderer), salir(false), tanqueSeleccionado(nullptr), turnoActual(0), tiempoRestante(50) { // 300s para los 5 min

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

    // Cargar las imágenes de los tanques
    tanqueRojo = IMG_LoadTexture(renderer, "/home/aldo-bagliano/Escritorio/TANK-ATTACK/Sprites/tanqueRojo.png");
    tanqueAzul = IMG_LoadTexture(renderer, "/home/aldo-bagliano/Escritorio/TANK-ATTACK/Sprites/tanqueAzul.png");
    tanqueAmarillo = IMG_LoadTexture(renderer, "/home/aldo-bagliano/Escritorio/TANK-ATTACK/Sprites/tanqueAmarillo.png");
    tanqueCeleste = IMG_LoadTexture(renderer, "/home/aldo-bagliano/Escritorio/TANK-ATTACK/Sprites/tanqueCeleste.png");

    if (!tanqueRojo || !tanqueAzul || !tanqueAmarillo || !tanqueCeleste) {
        std::cerr << "Error al cargar los sprites de los tanques: " << SDL_GetError() << std::endl;
        salir = true; // Si no se cargan los sprites, salir del juego
    }

    // Cargar la textura de la bala
    balaTextura = IMG_LoadTexture(renderer, "/home/aldo-bagliano/Escritorio/TANK-ATTACK/Sprites/bala.png");
    if (!balaTextura) {
        std::cerr << "Error al cargar el sprite de la bala: " << SDL_GetError() << std::endl;
        salir = true;
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

    // Liberar la textura de la bala
    SDL_DestroyTexture(balaTextura);
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

    if (evento->type == SDL_MOUSEBUTTONDOWN && evento->button.button == SDL_BUTTON_LEFT) { // Clic izquierdo
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Convertir las coordenadas del mouse a la celda del mapa
        int celdaX = mouseX / ancho;
        int celdaY = mouseY / alto;

        // Verificar si se hizo clic en un tanque del jugador actual
        for (auto& tanque : tanques) {
            if (tanque.x == celdaX && tanque.y == celdaY) {
                // Solo permitir la selección de tanques del jugador actual
                if ((turnoActual == 0 && (tanque.textura == tanqueRojo || tanque.textura == tanqueAzul)) ||
                    (turnoActual == 1 && (tanque.textura == tanqueAmarillo || tanque.textura == tanqueCeleste))) {
                    tanqueSeleccionado = &tanque; // Seleccionar el tanque
                    std::cout << "Tanque seleccionado en (" << tanque.x << ", " << tanque.y << ")" << std::endl;
                    return; // Salir después de seleccionar el tanque
                }
            }
        }

        // Si hay un tanque seleccionado, moverlo a la nueva posición
        if (tanqueSeleccionado != nullptr) {
            if (grafo->esCeldaLibre(celdaX, celdaY)) { // Verificar si la celda está libre
                tanqueSeleccionado->x = celdaX; // Mover el tanque a la nueva celda
                tanqueSeleccionado->y = celdaY;
                std::cout << "Tanque movido a (" << celdaX << ", " << celdaY << ")" << std::endl;
                tanqueSeleccionado = nullptr; // Desmarcar el tanque después de moverlo
                alternarTurno(); // Cambiar el turno después de mover
            } else {
                std::cout << "Celda ocupada, no se puede mover el tanque." << std::endl;
            }
        }
    }

    if (evento->type == SDL_MOUSEBUTTONDOWN && evento->button.button == SDL_BUTTON_RIGHT) { // Clic derecho
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        int celdaX = mouseX / ancho;
        int celdaY = mouseY / alto;

        // Si hay un tanque seleccionado, disparar una bala en esa dirección
        if (tanqueSeleccionado != nullptr) {
            int dx = celdaX - tanqueSeleccionado->x;
            int dy = celdaY - tanqueSeleccionado->y;

            // Normalizar la dirección
            if (dx != 0) dx = dx / abs(dx);
            if (dy != 0) dy = dy / abs(dy);

            // Crear una nueva bala
            balas.push_back(Bala(tanqueSeleccionado->x, tanqueSeleccionado->y, dx, dy, balaTextura));
            tanqueSeleccionado = nullptr; // Deseleccionar el tanque después de disparar
            alternarTurno(); // Cambiar el turno después de disparar
        }
    }
}

void Game::actualizar() {
    // Mover todas las balas
    for (auto& bala : balas) {
        bala.mover();
    }

    // Actualizar el tiempo restante
    Uint32 tiempoActual = SDL_GetTicks();
    if (tiempoActual - tiempoInicio >= 1000) { // Pasó un segundo
        tiempoRestante--;
        tiempoInicio = tiempoActual;

        if (tiempoRestante <= 0) {
            tiempoRestante = 0; // para que el tiempo no sea negativo
            determinarGanador();
        }
    }
}

void Game::renderizar() {
    // Renderizar los obstáculos del mapa
    grafo->dibujarGrafo(renderer);

    // Renderizar los tanques
    for (const auto& tanque : tanques) {
        SDL_Rect dstRect = { tanque.x * ancho, tanque.y * alto, ancho, alto };
        SDL_RenderCopy(renderer, tanque.textura, nullptr, &dstRect);
    }

    // Renderizar las balas
    for (const auto& bala : balas) {
        bala.renderizar(renderer, ancho, alto);
    }

    // Mostrar el turno actual y tiempo restante
    renderizarUI();
}

void Game::iniciar() {
    SDL_Event evento;
    tiempoInicio = SDL_GetTicks(); // Inicializar el tiempo

    while (!salir) {
        while (SDL_PollEvent(&evento)) {
            manejarEventos(&evento);
        }

        actualizar();

        SDL_RenderClear(renderer);
        renderizar();
        SDL_RenderPresent(renderer);
    }
}

void Game::alternarTurno() {
    turnoActual = (turnoActual + 1) % 2; // Alternar entre 0 y 1
    std::cout << "Turno del jugador " << (turnoActual + 1) << std::endl;
}

void Game::determinarGanador() {
    // Contar los tanques restantes de cada jugador
    int tanquesJugador1 = 0;
    int tanquesJugador2 = 0;

    for (const auto& tanque : tanques) {
        if (tanque.textura == tanqueRojo || tanque.textura == tanqueAzul) {
            tanquesJugador1++;
        } else if (tanque.textura == tanqueAmarillo || tanque.textura == tanqueCeleste) {
            tanquesJugador2++;
        }
    }

    // Determinar el ganador
    if (tanquesJugador1 > tanquesJugador2) {
        std::cout << "Jugador 1 gana!" << std::endl;
    } else if (tanquesJugador2 > tanquesJugador1) {
        std::cout << "Jugador 2 gana!" << std::endl;
    } else {
        std::cout << "Empate!" << std::endl;
    }

    salir = true; // Terminar el juego
}

void Game::renderizarUI() {
    // Inicializar TTF (solo la primera vez)
    if (TTF_Init() == -1) {
        SDL_Log("No se pudo inicializar SDL_ttf: %s", TTF_GetError());
        return;
    }

    // Cargar la fuente (asegúrate de tener un archivo .ttf)
    TTF_Font* fuente = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);  // Cambia la ruta a tu archivo de fuente
    if (!fuente) {
        SDL_Log("No se pudo cargar la fuente: %s", TTF_GetError());
        return;
    }

    // Colores de los textos
    SDL_Color colorBlanco = { 255, 255, 255, 255 };  // Blanco

    // Crear el texto del turno actual
    std::string textoTurno = (turnoActual == 0) ? "Turno: Jugador 1" : "Turno: Jugador 2";
    SDL_Surface* superficieTurno = TTF_RenderText_Solid(fuente, textoTurno.c_str(), colorBlanco);
    SDL_Texture* texturaTurno = SDL_CreateTextureFromSurface(renderer, superficieTurno);

    // Crear el texto del tiempo restante
    std::string textoTiempo = "Tiempo restante: " + std::to_string(tiempoRestante) + " s";
    SDL_Surface* superficieTiempo = TTF_RenderText_Solid(fuente, textoTiempo.c_str(), colorBlanco);
    SDL_Texture* texturaTiempo = SDL_CreateTextureFromSurface(renderer, superficieTiempo);

    // Posiciones en pantalla para mostrar los textos
    SDL_Rect rectTurno = { 10, 10, superficieTurno->w, superficieTurno->h };  // Posición para el turno
    SDL_Rect rectTiempo = { 10, 40, superficieTiempo->w, superficieTiempo->h };  // Posición para el tiempo restante

    // Renderizar los textos
    SDL_RenderCopy(renderer, texturaTurno, nullptr, &rectTurno);
    SDL_RenderCopy(renderer, texturaTiempo, nullptr, &rectTiempo);

    // Limpiar superficies y texturas
    SDL_FreeSurface(superficieTurno);
    SDL_FreeSurface(superficieTiempo);
    SDL_DestroyTexture(texturaTurno);
    SDL_DestroyTexture(texturaTiempo);

    // Cerrar la fuente y finalizar TTF
    TTF_CloseFont(fuente);
    TTF_Quit();
}


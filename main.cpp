#include <SDL2/SDL.h>
#include "UI/Menu.h"
#include "Game/Game.h"
#include <iostream>

enum class EstadoJuego {
    MENU,
    JUEGO,
    SALIR
};

int main(int argc, char* argv[]) {
    // Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Crear ventana y renderizador
    SDL_Window* window = SDL_CreateWindow("TANK ATTACK!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Error al crear la ventana: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Error al crear el renderizador: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Crear el menú y el juego
    Menu menu(renderer);
    Game* game = nullptr;

    EstadoJuego estado = EstadoJuego::MENU; // Inicializar el estado del juego
    bool salir = false;

    while (!salir) {
        SDL_Event evento;

        // Manejo de eventos (clics)
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) {
                salir = true;
            }

            // Manejar eventos según el estado
            if (estado == EstadoJuego::MENU) {
                menu.manejarEventos(&evento);

                // Iniciar el juego si se seleccionó
                if (menu.iniciarJuegoSeleccionado()) {
                    std::cout << "Iniciando juego..." << std::endl;
                    if (game == nullptr) {
                        game = new Game(renderer);  // Crear el objeto Game solo si no ha sido creado
                    }
                    estado = EstadoJuego::JUEGO; // Cambiar al estado de juego
                }

                // Salir si se seleccionó en el menú
                if (menu.salirSeleccionado()) {
                    salir = true;
                }
            } else if (estado == EstadoJuego::JUEGO && game) {
                game->manejarEventos(&evento);  // Manejar eventos dentro del juego
            }
        }

        // Actualizar y renderizar según el estado del juego
        if (estado == EstadoJuego::JUEGO && game) {
            game->actualizar();   // Actualizar el estado del juego (para que el tiempo corra bien )
        }

        // Renderizar según el estado
        SDL_RenderClear(renderer);
        if (estado == EstadoJuego::MENU) {
            menu.renderizar();  // Renderizar el menú
        } else if (estado == EstadoJuego::JUEGO && game) {
            game->renderizar();  // Renderizar el juego
        }
        SDL_RenderPresent(renderer);  // Presentar en pantalla

        SDL_Delay(16);  // Control de FPS (60 FPS aproximadamente)
    }

    // Limpiar memoria y cerrar SDL
    delete game; // Liberar la memoria del juego
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


    return 0;
}

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
    SDL_Window* window = SDL_CreateWindow("Menu Principal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
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
        while (SDL_PollEvent(&evento)) {
            // Manejar eventos
            if (evento.type == SDL_QUIT) {
                salir = true;
            }

            if (estado == EstadoJuego::MENU) {
                menu.manejarEventos(&evento);
                // Iniciar el juego si se seleccionó
                if (menu.iniciarJuegoSeleccionado()) {
                    std::cout << "Iniciando juego..." << std::endl; // Mensaje de inicio
                    if (game == nullptr) {
                        game = new Game(renderer); // Crear el objeto Game
                    }
                    estado = EstadoJuego::JUEGO; // Cambiar estado a juego
                }
                // Salir si se seleccionó
                if (menu.salirSeleccionado()) {
                    salir = true;
                }
            } else if (estado == EstadoJuego::JUEGO && game) {
                game->manejarEventos(&evento);
                game->actualizar();
                game->renderizar();

            }
        }

        // Renderizar según el estado
        SDL_RenderClear(renderer);
        if (estado == EstadoJuego::MENU) {
            menu.renderizar();
        } else if (estado == EstadoJuego::JUEGO && game) {
            game->renderizar();
        }
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // Para que sean 60 fps
    }

    // Limpiar memoria y cerrar SDL
    delete game; // Limpiar memoria del juego
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
